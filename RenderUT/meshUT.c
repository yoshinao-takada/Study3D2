#include "Render/mesh.h"
#include "Render/texture.h"
#include "Util/imagelogC.h"
#include "Util/testutils.h"
#include "Pspc/Pvector.h"
#include "NLSL/NLSLmatrix.h"
#include <memory.h>
#ifndef ARRAYSIZE
#define ARRAYSIZE(_a_) (sizeof(_a_) / sizeof((_a_)[0]))
#endif

// what to do
// 1) define mesh
// 2) define texture
// 3) test cross info
// 4) test getting texture value

#pragma region define_mesh
// 6 -- 7 -- 8
// |    |    |
// 3 -- 4 -- 5
// |    |    |
// 0 -- 1 -- 2
#define MESH_VERT_PROTO { \
    -1.0f, -1.0f, -1.0f, 1.0f, \
    0.0f, -1.0f, -1.0f, 1.0f, \
    1.0f, -1.0f, -1.0f, 1.0f, \
    -1.0f, 0.0f, -1.0f, 1.0f, \
    0.0f, 0.0f, -1.0f, 1.0f, \
    1.0f, 0.0f, -1.0f, 1.0f, \
    -1.0f, 1.0f, -1.0f, 1.0f, \
    0.0f, 1.0f, -1.0f, 1.0f, \
    1.0f, 1.0f, -1.0f, 1.0f }
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
static MeshTextureMapper_t texMapper = NULL_MESHTEXTUREMAPPER;
static TextureInterpolator_t interp = NULLTEXTUREINTERPOLATOR_C;

static int InitTexture()
{
    int err = EXIT_SUCCESS;
    do {
        if ((EXIT_SUCCESS != (err = PixelMap_fillstd(&textureSource, StdTex0))) ||
            (EXIT_SUCCESS != (err = TextureInterpolator_init(&interp, &textureSource))))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
        printf("interp.size = { %d, %d }\n", interp.size[0], interp.size[1]);
        
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

        // initialize texMapper
        if (EXIT_SUCCESS != (err = MeshTextureMapper_new(&texMapper, &mesh, &texMapperConf, &interp)))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}
#pragma endregion define_texture
#pragma region Testing_Mesh_cross
#define EYE_POSITION    { 0.0f, 0.0f, 1.0f, 1.0f }
#define LOOKAT  { \
    -0.9f, -0.95f, -1.0f, 1.0f, \
    0.95f, 0.9f, -1.0f, 1.0f, \
    -0.4f, -0.1f, -1.0f, 1.0f, \
    -1.1f, 0.0f, -1.0f, 1.0f }
static const float EyePosition[] = EYE_POSITION;
static const float LookAt[] = LOOKAT;

static int TextureMapperSimpleUT(pcMeshCrossInfo_t crossInfo, int iLookAtIndex)
{
    int err = EXIT_SUCCESS;
    do {
        float texValue = -100.0f;
        if (EXIT_SUCCESS != (err = MeshTextureMapper_get(&texMapper, crossInfo, &texValue)))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
       fprintf(stderr, "texValue = %f @ %s,%d\n", texValue, __FUNCTION__, __LINE__);
    } while (0);
    return err;
}

static int MeshCrossUT()
{
    const int LookAtCount = ARRAYSIZE(LookAt) / P3VSIZE;
    int err = EXIT_SUCCESS;
    float dir[P3VSIZE]; // view line direction vector work area
    MeshCrossInfo_t crossInfo;
    do {
        for (int iLookAtIndex = 0; iLookAtIndex != LookAtCount; iLookAtIndex++)
        {
            const float* lookAtCoord = LookAt + iLookAtIndex * P3VSIZE;
            P3V_sub(lookAtCoord, EyePosition, dir);
            int errTemp = Mesh_cross(&mesh, EyePosition, dir, &crossInfo);
            if ((iLookAtIndex != 3) && (errTemp != EXIT_SUCCESS))
            {
                err = errTemp;
                LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
            }
            else if ((iLookAtIndex == 3) && (errTemp == EXIT_SUCCESS))
            {
                err = EXIT_FAILURE;
                LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "outside point was not rejected.");
            }
            if (errTemp == EXIT_SUCCESS)
            {
                if (!TESTEQUALP3V(lookAtCoord, crossInfo.intersection, 1.0e-4f))
                {
                    err = EXIT_FAILURE;
                    MeshCrossInfo_show(stderr, &crossInfo);
                    LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "crossInfo mismatch");
                }
                if (EXIT_SUCCESS != (err = TextureMapperSimpleUT(&crossInfo, iLookAtIndex)))
                {
                    LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
                }
            }
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

#define RENDERED_WIDTH  640
#define RENDERED_HEIGHT 480
#define RENDERED_XC     (RENDERED_WIDTH/2)
#define RENDERED_YC     (RENDERED_HEIGHT/2)
static int MeshCross2UT()
{
    static const Size2iC_t RenderedSize = { RENDERED_WIDTH, RENDERED_HEIGHT };
    static const RectC_t RenderedROI = { 0, 0, RENDERED_WIDTH, RENDERED_HEIGHT };
    int err = EXIT_SUCCESS;
    ImageC_t rendered = NULLIMAGE_C;
    MeshCrossInfo_t crossInfo;
    do {
        if (EXIT_SUCCESS != (err = ImageC_New(&rendered, RenderedSize, RenderedROI)))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
        for (int irow = 0; irow != rendered.size[1]; irow++)
        {
            for (int icol = 0; icol != rendered.size[0]; icol++)
            {
                float lookAt[] = { (icol - RENDERED_XC) * 0.01f, (irow - RENDERED_YC) * 0.01f, -1.0f, 1.0f };
                float dir[P3VSIZE];
                P3V_sub(lookAt, EyePosition, dir);
                if (EXIT_SUCCESS != Mesh_cross(&mesh, EyePosition, dir, &crossInfo)) continue;
                int iLinearIndex = icol + irow * rendered.size[0];
                if (EXIT_SUCCESS != MeshTextureMapper_get(&texMapper, &crossInfo, &rendered.elements[iLinearIndex]))
                {
                    LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
                }
            }
        }
        ImageLog_ShowImageRange(&rendered, 0.0f, 1.0f);
    } while (0);
    ImageC_Delete(&rendered);
    return err;
}

#pragma endregion Testing_Mesh_cross
static void Cleanup()
{
    Mesh_delete(&mesh);
    MeshTextureMapperConf_delete(&texMapperConf);
    MeshTextureMapper_delete(&texMapper);
    ImageC_Delete(&textureSource);
}

int meshUT()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = InitMesh()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
        if (EXIT_SUCCESS != (err = InitTexture()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
        if (EXIT_SUCCESS != (err = MeshCrossUT()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
        if (EXIT_SUCCESS != (err = MeshCross2UT()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    Cleanup();
    return 0;
}