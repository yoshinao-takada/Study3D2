#include "Render/projection.h"

int Projection_render(pcScene_t scene, pImageC_t viewport)
{
    int err = EXIT_SUCCESS;
    float p0[P3VSIZE], dir[P3VSIZE];
    MeshCrossInfo_t crossinfo;
    MeshTextureMapper_t texMapper = NULL_MESHTEXTUREMAPPER;
    TextureInterpolator_t interpolator = NULLTEXTUREINTERPOLATOR_C;
    do {
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
        for (int row = 0; row < viewport->size[1]; row++)
        {
            for (int col = 0; col < viewport->size[0]; col++)
            {
                int linearIndex = col + row * viewport->size[0];
                float* pixel = viewport->elements + linearIndex;
                const float vpXY[] = { (float)col, (float)row };
                Camera2_viewline(scene->camera, vpXY, p0, dir);
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