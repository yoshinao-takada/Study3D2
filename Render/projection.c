#include "Render/projection.h"
#include "Pspc/Pvector.h"
#include "NLSL/NLSLmatrix.h"
#include <errno.h>

// /**
//  * @brief make a projection matrix
//  * 
//  * @param intrin [in] camera intrinsic matrix
//  * @param extrin [in] camera extrinsic matrix
//  * @param proj [out] resulted projection matrix
//  */
// static void MakeProjMat(const float *intrin, const float* extrin, float* proj)
// {
//     NLSLmatrix_t intrin_ = { P2VSIZE, P3VSIZE, {intrin} };
//     NLSLmatrix_t extrin_ = { P3VSIZE, P3VSIZE, {extrin} };
//     NLSLmatrix_t proj_ = { P2VSIZE, P3VSIZE, {proj} };
//     NLSLmatrix_mult(&intrin_, &extrin_, &proj_);
// }

int Projection_render(pcScene_t scene, pImageC_t viewport)
{
    int err = EXIT_SUCCESS;
    float p0[P3VSIZE], dir[P3VSIZE], matProj[P2VSIZE * P3VSIZE], matIntrin[P2VSIZE * P3VSIZE],
        matExtrin[P3MSIZE];
    MeshCrossInfo_t crossinfo;
    MeshTextureMapper_t texMapper = NULL_MESHTEXTUREMAPPER;
    TextureInterpolator_t interpolator = NULLTEXTUREINTERPOLATOR_C;
    Mesh_t mesh = NULLMESH;
    const P3MCameraPosition_t campos =
    {
        0, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f, 1.0f }
    };
    do {
        // Initialize viewport
        if (viewport->size[0] != scene->cameraConf->size[0] || viewport->size[1] != scene->cameraConf->size[1])
        {
            ImageC_Delete(viewport);
            Size2iC_t size = { scene->cameraConf->size[0], scene->cameraConf->size[1] };
            RectC_t roi = { 0, 0, size[0], size[1] };
            if (EXIT_SUCCESS != (err = ImageC_New(viewport, size, roi)))
            {
                err = ENOMEM;
                break;
            }
        }

        // move the mesh to the camera coord
        if (EXIT_SUCCESS != (err = Mesh_new(&mesh, scene->geometryModel->nvert, scene->geometryModel->ntri)))
        {
            break;
        }
        P3M_tocameracoord(scene->cameraPosition, matExtrin);
        Mesh_transform(scene->geometryModel, &mesh, matExtrin); // move mesh to camera default coordinate

        // create textutre interpolator and texture mapper
        if (EXIT_SUCCESS != (err = TextureInterpolator_init(&interpolator, scene->texture)))
        {
            fprintf(stderr, "err = %d @ %s,%d\n", err, __FILE__, __LINE__);
            break;
        }
        if (EXIT_SUCCESS != (err = MeshTextureMapper_new(&texMapper, &mesh, scene->texConf, &interpolator)))
        {
            fprintf(stderr, "err = %d @ %s,%d\n", err, __FILE__, __LINE__);
            break;
        }
        // scan camera pixels
        Camera35mmConf_mat(scene->cameraConf, matIntrin);
        for (int row = 0; row < viewport->size[1]; row++)
        {
            for (int col = 0; col < viewport->size[0]; col++)
            {
                int linearIndex = col + row * viewport->size[0];
                float* pixel = viewport->elements + linearIndex;
                const float vpXY[] = { (float)col, (float)row };
                CameraViewline(matIntrin, vpXY, &campos, p0, dir);
                if (EXIT_SUCCESS != Mesh_cross(&mesh, p0, dir, &crossinfo)) continue;
                MeshTextureMapper_get(&texMapper, &crossinfo, pixel);
            }
        }
    } while(0);
    Mesh_delete(&mesh);
    MeshTextureMapper_delete(&texMapper);
    TextureInterpolator_delete(&interpolator);
    return err;
}

static void DeleteMeshArray(int size, pMesh_t meshes)
{
    if (!meshes) return;
    for (int i = 0; i < size; i++)
    {
        Mesh_delete(meshes + i);
    }
    free(meshes);
}

static void DeleteTextureMapperArray(int size, pMeshTextureMapper_t textureMappers)
{
    if (!textureMappers) return;
    for (int i = 0; i < size; i++)
    {
        TextureInterpolator_delete((pTextureInterpolator_t)textureMappers[i].texsrc);
        MeshTextureMapper_delete(textureMappers + i);
    }
    free((pTextureInterpolator_t)textureMappers[0].texsrc);
    free(textureMappers);
}

static int InitViewport(pcCamera35mmConf_t cameraConf, pImageC_t viewport)
{
    const RectC_t roi = { 0, 0, (int)(cameraConf->size[0] + 0.5f), (int)(cameraConf->size[1] + 0.5f) };
    const Size2iC_t size = { roi[2], roi[3] };
    return ImageC_New(viewport, size, roi);
}

typedef struct {
    MeshCrossInfo_t crossInfo;
    int result;
} MeshCrossResult_t, *pMeshCrossResult_t;
typedef const MeshCrossResult_t *pcMeshCrossResult_t;

pMesh_t Projection_instantiate(pcScene2_t scene)
{
    float cameraExtrinsicMat[P3MSIZE], combinedMat_[P3MSIZE];
    P3M_tocameracoord(scene->cameraPosition, cameraExtrinsicMat); // create camera extrinsic matrix
    pMesh_t meshes = (pMesh_t)calloc(scene->objCount, sizeof(Mesh_t));
    if (!meshes)
    {
        return meshes;
    }
    for (int iSceneObject = 0; iSceneObject < scene->objCount; iSceneObject++)
    {
        NLSLmatrix_t cameraMat = { P3MROWS, P3MCOLUMNS, { cameraExtrinsicMat} };
        NLSLmatrix_t combinedMat = { P3MROWS, P3MCOLUMNS, { combinedMat_ } };
        NLSLmatrix_t objectTransform = { P3MROWS, P3MCOLUMNS, { scene->objects[iSceneObject].transform } };
        NLSLmatrix_mult(&cameraMat, &objectTransform, &combinedMat);
        pMesh_t pmesh = meshes + iSceneObject;
        int nvert = scene->objects[iSceneObject].geometryModel->nvert;
        int ntri = scene->objects[iSceneObject].geometryModel->ntri;
        if (EXIT_SUCCESS != Mesh_new(pmesh, nvert, ntri))
        {
            for (int i = 0; i < iSceneObject; i++)
            {
                Mesh_delete(meshes + i);
            }
            free(meshes);
            break;
        }
        Mesh_transform(scene->objects[iSceneObject].geometryModel, meshes + iSceneObject, combinedMat_);
    }
    return meshes;
}

pMeshTextureMapper_t Projection_textureMapper(pcScene2_t scene, pcMesh_t meshes)
{
    pMeshTextureMapper_t textureMappers = (pMeshTextureMapper_t)NULL;
    pTextureInterpolator_t interpolators = (pTextureInterpolator_t)NULL;
    textureMappers = (pMeshTextureMapper_t)calloc(scene->objCount, sizeof(MeshTextureMapper_t));
    interpolators = (pTextureInterpolator_t)calloc(scene->objCount, sizeof(TextureInterpolator_t));
    for (int iSceneObject = 0; iSceneObject < scene->objCount; iSceneObject++)
    {
        if (TextureInterpolator_init(interpolators + iSceneObject, scene->objects[iSceneObject].texture))
        {
            fprintf(stderr, "fail in TextureInterpolator_init(%d,) @ %s,%d\n", iSceneObject, __FILE__, __LINE__);
            exit(1);
        }
        if (MeshTextureMapper_new(textureMappers + iSceneObject, meshes + iSceneObject,
            scene->objects[iSceneObject].texConf, interpolators + iSceneObject))
        {
            exit(1);
        }
    }
    return textureMappers;
}

// Step 1: instantiate geometry models
// Step 2: create texture mapper
// Step 3: scan viewline
int Projection_render2(pcScene2_t scene, pImageC_t viewport)
{
    int err = EXIT_SUCCESS;
    pMesh_t meshes = (pMesh_t)NULL;
    pMeshTextureMapper_t textureMappers = (pMeshTextureMapper_t)NULL;
    pTextureInterpolator_t interpolators = (pTextureInterpolator_t)NULL;
    float cameraIntrinsicMat[P2VSIZE * P3VSIZE];
    const P3MCameraPosition_t campos =
    {
        0, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f, 1.0f }
    };
    pMeshCrossResult_t crossResults = (pMeshCrossResult_t)NULL;
    do {
        // Step 1: instantiate geometry models
        if (NULL == (meshes = Projection_instantiate(scene)))
        {
            err = ENOMEM;
            break;
        }

        // Step 2: create texture mapper
        if (NULL == (textureMappers = Projection_textureMapper(scene, meshes)))
        {
            err = ENOMEM;
            break;
        }        

        // Step 3: scan viewlines
        if (EXIT_SUCCESS != (err = InitViewport(scene->cameraConf, viewport)))
        {
            break;
        }
        crossResults = (pMeshCrossResult_t)calloc(scene->objCount, sizeof(MeshCrossResult_t));
        for (int iSceneObect = 0; iSceneObect < scene->objCount; iSceneObect++)
        {
            crossResults[iSceneObect].crossInfo.mesh = meshes + iSceneObect;
        }
        Camera35mmConf_mat(scene->cameraConf, cameraIntrinsicMat);
        for (int row = 0; row < viewport->size[1]; row++)
        {
            for (int col = 0; col < viewport->size[0]; col++)
            {
                float p0[P3VSIZE], dir[P3VSIZE];
                MeshCrossResult_t crossResult;
                int linearIndex = col + row * viewport->size[0];
                float* pixel = viewport->elements + linearIndex;
                const float vpXY[] = { (float)col, (float)row };
                CameraViewline(cameraIntrinsicMat, vpXY, &campos, p0, dir);
                for (int iSceneObject = 0; iSceneObject < scene->objCount; iSceneObject++)
                {
                    crossResults[iSceneObject].result = Mesh_cross(meshes + iSceneObject, p0, dir, &crossResults[iSceneObject].crossInfo);
                }
                int nearest = -1;
                float znearest = -__FLT_MAX__;
                for (int iSceneObject = 0; iSceneObject < scene->objCount; iSceneObject++)
                {
                    if (crossResults[iSceneObject].result) continue;
                    float z = crossResults[iSceneObject].crossInfo.intersection[2] / crossResults[iSceneObject].crossInfo.intersection[3];
                    if (z > znearest)
                    {
                        nearest = iSceneObject;
                        znearest = z;
                    }
                }
                if (nearest >= 0)
                {
                    MeshTextureMapper_get(textureMappers + nearest, &crossResults[nearest].crossInfo, pixel);
                }
            }
        }
    } while (0);
    DeleteMeshArray(scene->objCount, meshes);
    DeleteTextureMapperArray(scene->objCount, textureMappers);
    free(crossResults);
    return err;
}