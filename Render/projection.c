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
        0, { 0.0f, 0.0f, 0.0f, 1.0f },
        {
            scene->cameraPosition->lookat[0] - scene->cameraPosition->position[0],
            scene->cameraPosition->lookat[1] - scene->cameraPosition->position[1],
            scene->cameraPosition->lookat[2] - scene->cameraPosition->position[2],
            scene->cameraPosition->lookat[3] - scene->cameraPosition->position[3],
        }
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