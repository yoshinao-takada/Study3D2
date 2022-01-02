#include "Render/projection.h"
#include "Pspc/Pvector.h"
#include "NLSL/NLSLmatrix.h"
#include <errno.h>

/**
 * @brief make a projection matrix
 * 
 * @param intrin [in] camera intrinsic matrix
 * @param extrin [in] camera extrinsic matrix
 * @param proj [out] resulted projection matrix
 */
static void MakeProjMat(const float *intrin, const float* extrin, float* proj)
{
    NLSLmatrix_t intrin_ = { P2VSIZE, P3VSIZE, {intrin} };
    NLSLmatrix_t extrin_ = { P3VSIZE, P3VSIZE, {extrin} };
    NLSLmatrix_t proj_ = { P2VSIZE, P3VSIZE, {proj} };
    NLSLmatrix_mult(&intrin_, &extrin_, &proj_);
}

int Projection_render(pcScene_t scene, pImageC_t viewport)
{
    int err = EXIT_SUCCESS;
    float p0[P3VSIZE], dir[P3VSIZE], matProj[P2VSIZE * P3VSIZE], matIntrin[P2VSIZE * P3VSIZE],
        matExtrin[P3MSIZE];
    MeshCrossInfo_t crossinfo;
    MeshTextureMapper_t texMapper = NULL_MESHTEXTUREMAPPER;
    TextureInterpolator_t interpolator = NULLTEXTUREINTERPOLATOR_C;
    do {
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
        err = TextureInterpolator_init(&interpolator, scene->texture);
        if (EXIT_SUCCESS != err)
        {
            fprintf(stderr, "err = %d @ %s,%d\n", err, __FILE__, __LINE__);
            break;
        }
        err = MeshTextureMapper_new(&texMapper, scene->geometryModel, scene->texConf, &interpolator);
        if (EXIT_SUCCESS != err)
        {
            fprintf(stderr, "err = %d @ %s,%d\n", err, __FILE__, __LINE__);
            break;
        }
        P3M_tocameracoord(scene->cameraPosition, matExtrin);
        Camera35mmConf_mat(scene->cameraConf, matIntrin);
        MakeProjMat(matIntrin, matExtrin, matProj);
        for (int row = 0; row < viewport->size[1]; row++)
        {
            for (int col = 0; col < viewport->size[0]; col++)
            {
                int linearIndex = col + row * viewport->size[0];
                float* pixel = viewport->elements + linearIndex;
                const float vpXY[] = { (float)col, (float)row };
                CameraViewline(matProj, vpXY, scene->cameraPosition, p0, dir);
                if (EXIT_SUCCESS != Mesh_cross(scene->geometryModel, p0, dir, &crossinfo)) continue;
                MeshTextureMapper_get(&texMapper, &crossinfo, pixel);
            }
        }
        err = EXIT_SUCCESS;
    } while (0);
    MeshTextureMapper_delete(&texMapper);
    TextureInterpolator_delete(&interpolator);
    return err;
}