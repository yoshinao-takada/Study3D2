#include "Render/texture.h"
#include "Util/testutils.h"
#include "Util/imagelogC.h"

static ImageC_t textureSource = NULLIMAGE_C;
static const Size2iC_t TextureSize = { 600, 400 }; // horizontal 12 block, vertical 8 block
static const RectC_t TextureROI = { 0, 0, 600, 400 };
static const Size2iC_t TextureBlockSize = { 50, 50 };

static const Size2iC_t MiniSize = { 6, 6 };
static const RectC_t MiniROI = { 0, 0, 6, 6 };

static int textureInitUT()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (PixelMap_fillstd(&textureSource, StdTex0)))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
        ImageLog_ShowImageRange(&textureSource, 0.0f, 1.0f);
    } while (0);
    ImageC_Delete(&textureSource);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

static int textureInterpolateUT()
{
    TextureInterpolator_t interpolator = NULLTEXTUREINTERPOLATOR_C;
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = ImageC_New(&textureSource, MiniSize, MiniROI)))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
        for (int iy = 0; iy != MiniSize[1]; iy++)
        {
            for (int ix = 0; ix != MiniSize[0]; ix++)
            {
                int linearIndex = ix + iy * textureSource.size[0];
                textureSource.elements[linearIndex] = (float)(linearIndex);
            }
        }
        if (EXIT_SUCCESS != (err = TextureInterpolator_init(&interpolator, &textureSource)))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
        Size2iC_t subsize = { 100, 100 };
        float delta[2] = { 1.0f/(float)subsize[0], 1.0f/(float)subsize[1] };
        for (int32_t iy = 0; iy != subsize[1] * interpolator.size[1]; iy++)
        {
            for (int32_t ix = 0; ix != subsize[0] * interpolator.size[0]; ix++) 
            {
                float xy[2] = { ix * delta[0], iy * delta[1] };
                float value = -1000.0f;
                if (EXIT_SUCCESS != (err = TextureInterpolator_get(&interpolator, xy, &value)))
                {
                    LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
                }
                fprintf(stderr, "value(%f,%f)=%f @ %s,%d\n", xy[0], xy[1], value, __FUNCTION__, __LINE__);
            }
        }
    } while (0);
    TextureInterpolator_delete(&interpolator);
    ImageC_Delete(&textureSource);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

int textureUT()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = textureInitUT()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
        if (EXIT_SUCCESS != (err = textureInterpolateUT()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}
