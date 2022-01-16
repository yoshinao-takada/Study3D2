#include "Render/projection.h"
#include "Util/testutils.h"
#include "Util/imagelogC.h"
#include "Pspc/Pvector.h"
#include "NLSL/NLSLmatrix.h"
#include <memory.h>
#ifndef ARRAYSIZE
#define ARRAYSIZE(_a_) (sizeof(_a_) / sizeof((_a_)[0]))
#endif

#pragma region define_mesh
// 6 -- 7 -- 8
// |    |    |
// 3 -- 4 -- 5
// |    |    |
// 0 -- 1 -- 2
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

static const float MeshVertProto[] = MESH_VERT_PROTO;
static const int32_t MeshTriProto[] = MESH_TRI_PROTO;
static const float MeshTextureMapperConfProto[] = MESHTEXTUREMAPPERCONF_PROTO;

static Mesh_t mesh = NULLMESH;
static int InitMesh()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (Mesh_new(&mesh, ARRAYSIZE(MeshVertProto)/P3VSIZE, ARRAYSIZE(MeshTriProto)/3)))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
        memcpy(mesh.vertices, MeshVertProto, sizeof(MeshVertProto));
        memcpy(mesh.indices, MeshTriProto, sizeof(MeshTriProto));
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}
#pragma endregion define_mesh
#pragma region define_texture
static ImageC_t textureSource = NULLIMAGE_C;
static MeshTextureMapperConf_t texMapperConf = NULL_MESHTEXTUREMAPPERCONF;
static MeshTextureMapper_t mapper = NULL_MESHTEXTUREMAPPER;
static TextureInterpolator_t interpolator = NULLTEXTUREINTERPOLATOR_C;

static int InitTexture()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = PixelMap_fillstd(&textureSource, StdTex0)))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
        
        // initialize texMapperConf
        if (EXIT_SUCCESS != (err = MeshTextureMapperConf_new(&texMapperConf, &mesh)))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
        const int32_t* ptrTri = mesh.indices;
        float* texCoords = texMapperConf.textureCoords;
        for (int itri = 0; itri != mesh.ntri; itri++)
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
            ptrTri += 3;
        }
        if (EXIT_SUCCESS != (err = TextureInterpolator_init(&interpolator, &textureSource)))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
        if (EXIT_SUCCESS != (err = MeshTextureMapper_new(&mapper, &mesh, &texMapperConf, &interpolator)))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}
#pragma endregion define_texture
static ImageC_t renderTarget = NULLIMAGE_C;
#define RENDER_WIDTH    640
#define RENDER_HEIGHT   480
#define RENDER_FOCALLEN 22.0e-3f
#define CAMERAPOS   { 3.0f, 0.0f, 1.5f, 1.0f }
#define LOOKAT      { 0.0f, 0.0f, 0.0f, 1.0f }
static float matProj[P2VSIZE * P3VSIZE];
static P3MCameraPosition_t cameraPos = { 0.0f, CAMERAPOS, LOOKAT };
static const Camera35mmConf_t cameraConf = { { RENDER_WIDTH, RENDER_HEIGHT }, RENDER_FOCALLEN };

static void Cleanup()
{
    MeshTextureMapperConf_delete(&texMapperConf);
    ImageC_Delete(&textureSource);
    Mesh_delete(&mesh);
    ImageC_Delete(&renderTarget);
}

static float EquivalentFocalLength(float width, float height)
{
    const float diagonal35mm = 43.27f;
    const float equivalentForcalLength35mm = 22.0f;
    const float diagonal = sqrtf(width*width + height*height);
    return diagonal * equivalentForcalLength35mm / diagonal35mm;
}

static void InitMatProj()
{
    float matIntrin[P2VSIZE * P3VSIZE], matExtrin[P3MSIZE];
    NLSLmatrix_t matIntrin_ = { P2VSIZE, P3VSIZE, { matIntrin } };
    NLSLmatrix_t matExtrin_ = { P3VSIZE, P3VSIZE, { matExtrin } };
    NLSLmatrix_t matProj_ = { P2VSIZE, P3VSIZE, { matProj } };
    Camera35mmConf_mat(&cameraConf, matIntrin);
    P3M_tocameracoord(&cameraPos, matExtrin);
    NLSLmatrix_mult(&matIntrin_, &matExtrin_, &matProj_);
}

static Scene_t scene = { NULL, NULL, NULL, NULL, NULL };

int projectionUT()
{
    int err = EXIT_SUCCESS;
    static const Size2iC_t renderSize = { RENDER_WIDTH, RENDER_HEIGHT };
    static const RectC_t renderRoi = { 0, 0, RENDER_WIDTH, RENDER_HEIGHT };
    do {
        if ((EXIT_SUCCESS != (err = InitMesh())) ||
            (EXIT_SUCCESS != (err = InitTexture())))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
        scene.geometryModel = &mesh;
        scene.texture = &textureSource;
        scene.texConf = &texMapperConf;
        scene.cameraConf = &cameraConf;
        scene.cameraPosition = &cameraPos;

        ImageC_Delete(&renderTarget);
        if (EXIT_SUCCESS != (err = Projection_render(&scene, &renderTarget)))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
        ImageLog_ShowImageRange(&renderTarget, 0.0f, 1.0f);
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    Cleanup();
    return err;
}

