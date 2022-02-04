#include "Render/projection.h"
#include "Util/testutils.h"
#include "Util/imagelogC.h"
#include "Pspc/Pvector.h"
#include "NLSL/NLSLmatrix.h"
#include "NLSL/NLSLutils.h"
#include <memory.h>
#include <math.h>

#pragma region define_mesh
// 6 -- 7 -- 8
// |    |    |
// 3 -- 4 -- 5
// |    |    |
// 0 -- 1 -- 2
// base mesh (x = [-2, 2] on z-plane)
#define MESH_VERT_PROTO { \
    -1.0f, -1.0f, 0.0f, 1.0f, \
    0.0f, -1.0f, 0.0f, 1.0f, \
    1.0f, -1.0f, 0.0f, 1.0f, \
    -1.0f, 0.0f, 0.0f, 1.0f, \
    0.0f, 0.0f, 0.0f, 1.0f, \
    1.0f, 0.0f, 0.0f, 1.0f, \
    -1.0f, 1.0f, 0.0f, 1.0f, \
    0.0f, 1.0f, 0.0f, 1.0f, \
    1.0f, 1.0f, 0.0f, 1.0f }
#define MESH_TRI_PROTO { \
    0, 4, 3, \
    0, 1, 4, \
    1, 5, 4, \
    1, 2, 5, \
    3, 7, 6, \
    3, 4, 7, \
    4, 8, 7, \
    4, 5,  8 }

// (0,0) -- (.5,0) -- (1.,0)
//   |         |         |
// (0,.5) - (.5,.5) - (1.,.5)
//   |         |         |
// (0,1.) - (.5,1.) - (1.,1.)
#define MESHTEXTUREMAPPERCONF_PROTO { \
    0.0f, 0.0f,  0.5f, 0.0f,  1.0f, 0.0f, \
    0.0f, 0.5f,  0.5f, 0.5f,  1.0f, 0.5f, \
    0.0f, 1.0f,  0.5f, 1.0f,  1.0f, 1.0f }

static float MeshVertProto[] = MESH_VERT_PROTO;
static int32_t MeshTriProto[] = MESH_TRI_PROTO;
static const float MeshTextureMapperConfProto[] = MESHTEXTUREMAPPERCONF_PROTO;
static const Mesh_t MeshProto = { 9, 8, MeshVertProto, MeshTriProto };
static MeshTextureMapperConf_t MapperConf = NULL_MESHTEXTUREMAPPERCONF;
static SceneObject_t objects[] = {
    { NULL, NULL, NULL, { 0.0f }}, // back wall
    { NULL, NULL, NULL, { 0.0f }}, // left wall
    { NULL, NULL, NULL, { 0.0f }}, // floor
    { NULL, NULL, NULL, { 0.0f }} // target
};

static ImageC_t WallTexture = NULLIMAGE_C;
static ImageC_t TargetTexture = NULLIMAGE_C;
static ImageC_t RenderTarget = NULLIMAGE_C;
static const float _pi_f = __f32 (3.141592653589793238462643383279502884);

static void BackWallTransform(float* mat)
{
    static const float x2[] = {
        2.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 2.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    static const float move[] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, -2.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    float work0[P3MSIZE], work1[P3MSIZE];
    P3M_mult(move, P3M_mult(x2, P3M_rotateZ(0.5f * _pi_f, work0), work1), mat);
}

static void LeftWallTransform(float* mat)
{
    static const float x2[] = {
        2.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 2.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    static const float move[] = {
        1.0f, 0.0f, 0.0f, -2.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    float work0[P3MSIZE], work1[P3MSIZE];
    P3M_mult(move, P3M_mult(x2, P3M_rotateY(0.5f * _pi_f, work0), work1), mat);
}

static void BottomWallTransform(float* mat)
{
    static const float x2[] = {
        2.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 2.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    static const float move[] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -2.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    float work0[P3MSIZE], work1[P3MSIZE];
    P3M_mult(move, P3M_mult(x2, P3M_rotateX(0.5f * _pi_f, work0), work1), mat);
}

static void TargetPosition(float* mat)
{
    NLSL_FILLFLOATS(mat, 0.0f, P3MSIZE);
    mat[0] = mat[5] = mat[10] = mat[15] = 1.0f; // set diagonal elements to make an identity matrix
}

static int InitSceneObjects()
{
    int err = EXIT_SUCCESS;
    do {
        // set transform matrices
        BackWallTransform(objects[0].transform);
        LeftWallTransform(objects[1].transform);
        BottomWallTransform(objects[2].transform);
        TargetPosition(objects[3].transform);

        // set mesh prototype
        objects[0].geometryModel = objects[1].geometryModel =
        objects[2].geometryModel =  objects[3].geometryModel = &MeshProto;

        // init textures
        PixelMap_fillstd(&WallTexture, StdTex1);
        PixelMap_fillstd(&TargetTexture, StdTex0);
        objects[0].texture = objects[1].texture = objects[2].texture = &WallTexture;
        objects[3].texture = &TargetTexture;

        // init texture mapper conf
        if (EXIT_SUCCESS != (err = MeshTextureMapperConf_new(&MapperConf, &MeshProto)))
        {
            break;
        }
        const int32_t* ptrTri = MeshProto.indices;
        float* texCoords = MapperConf.textureCoords;
        for (int itri = 0; itri != MeshProto.ntri; itri++)
        {
            int vertIndex = ptrTri[0];
            const float* texCoordSrc = MeshTextureMapperConfProto + vertIndex * 2;
            COPY2(texCoords, texCoordSrc);
            texCoords += 2;
            vertIndex = ptrTri[1];
            texCoordSrc = MeshTextureMapperConfProto + vertIndex * 2;
            COPY2(texCoords, texCoordSrc);
            texCoords += 2;
            vertIndex = ptrTri[2];
            texCoordSrc = MeshTextureMapperConfProto + vertIndex * 2;
            COPY2(texCoords, texCoordSrc);
            texCoords += 2;
            ptrTri += 3; // goto next triangle
        }
        objects[0].texConf = objects[1].texConf = objects[2].texConf = objects[3].texConf = &MapperConf;
    } while (0);
    return err;
}

static void Cleanup()
{
    MeshTextureMapperConf_delete(&MapperConf);
    ImageC_Delete(&WallTexture);
    ImageC_Delete(&TargetTexture);
    ImageC_Delete(&RenderTarget);
}

static const Camera35mmConf_t CameraParams = { { 640, 480 }, 22.0e-3f };

static const P3MCameraPosition_t CameraPosition = {
    0.0f, { 3.0f, 3.0f, 3.0f, 1.0f }, { 0.0f, 0.0f, 0.5f, 1.0f }
};

static const Scene2_t scene = { 4, objects, &CameraParams, &CameraPosition };

static void ShowMeshes(int count, pcMesh_t meshes)
{
    fprintf(stderr, "MeshCount = %d\n", count);
    for (int i = 0; i < count; i++)
    {
        fprintf(stderr, "Mesh[%d].nvert = %d\n", i, meshes[i].nvert);
        for (int iVert = 0; iVert < meshes[i].nvert; iVert++)
        {
            const float* vert = meshes[i].vertices + iVert * 4;
            fprintf(stderr, "Mesh[%d].vert[%d] = { %f, %f, %f (%f) }\n", i, iVert,
                vert[0]/vert[3], vert[1]/vert[3], vert[2]/vert[3], vert[3]);
        }
        for (int iTri = 0; iTri < meshes[i].ntri; iTri++)
        {
            const int32_t* tri_indices = meshes[i].indices + iTri * 3;
            fprintf(stderr, "Mesh[%d].tri[%d] = { %d, %d, %d }\n", i, iTri,
                tri_indices[0], tri_indices[1], tri_indices[2]);
        }
    }
}

void CleanMeshes(int count, pMesh_t meshes)
{
    if (meshes)
    {
        for (int i = 0; i < count; i++)
        {
            Mesh_delete(meshes + i);
        }
        free(meshes);
    }
}

void CleanTextureMappers(int count, pMeshTextureMapper_t textureMappers)
{
    if (!textureMappers) return;
    for (int i = 0; i < count; i++)
    {
        TextureInterpolator_delete((pTextureInterpolator_t)textureMappers[i].texsrc);
        MeshTextureMapper_delete(textureMappers + i);
    }
    free(textureMappers);
}

int projection2UT()
{
    int err = EXIT_SUCCESS;
    pMesh_t meshes = NULL;
    pMeshTextureMapper_t texMappers = NULL;
    do {
        if (EXIT_SUCCESS != (err = InitSceneObjects()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
        if (EXIT_SUCCESS != (err = Projection_render2(&scene, &RenderTarget)))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
        ImageLog_ShowImageRange(&RenderTarget, 0.0f, 1.0f);
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    CleanMeshes(scene.objCount, meshes);
    CleanTextureMappers(scene.objCount, texMappers);
    Cleanup();
    return err;
}