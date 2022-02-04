#include "Render/texture.h"
#include "Util/testutils.h"
#include "Util/imagelogC.h"
#include "Util/FastHistogram.h"
#include "NLSL/NLSLutils.h"
#include "Render/StdTexDefs.h"

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

static int textureBlobUT()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (PixelMap_SolidAndBlobs(&textureSource, StdTex1)))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
        ImageLog_ShowImageRange(&textureSource, 0.0f, 1.0f);
        if (EXIT_SUCCESS != (PixelMap_SolidAndBlobs(&textureSource, StdTex1)))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
        ImageLog_ShowImageRange(&textureSource, 0.0f, 1.0f);
        if (EXIT_SUCCESS != (PixelMap_SolidAndBlobs(&textureSource, StdTex1)))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
        ImageLog_ShowImageRange(&textureSource, 0.0f, 1.0f);
    } while (0);
    ImageC_Delete(&textureSource);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

static int textureBlob2UT()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = PixelMap_fillstd(&textureSource, StdTex1)))
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

static int texture_RangedRandUT()
{
    int err = EXIT_SUCCESS;
    int intHist[20]; // histogram of ranged int numbers
    int RangeI[] = { -5, 5 };
    do {
        NLSL_FILLINTS(intHist, 0, NLSL_ARRAYSIZE(intHist)); // clear intHist
        for (int i = 0; i < 100; i++)
        {
            for (int j = 0; j < 100; j++)
            {
                int n = Texture_RangedRand(RangeI[0], RangeI[1]);
                n += 10;
                intHist[n]++;
            }
        }
        int sum = 0;
        int i = 0;
        for (; i < 5; i++)
        {
            sum += intHist[i];
        }
        if (sum)
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Texture_RangedRand() failed.");
        }
        for (; i < 15; i++)
        {
            sum += intHist[i];
        }
        if (sum != 10000)
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Texture_RangedRand() failed.");
        }
        for (; i < NLSL_ARRAYSIZE(intHist); i++)
        {
            sum += intHist[i];
        }
        if (sum != 10000)
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Texture_RangedRand() failed.");
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

int texture_RangedRandF_UT()
{
    static const float range[] = { -2.5f, 0.5f };
    int err = EXIT_SUCCESS;
    FastHistogram_t hist = NULLFASTHISTOGRAM_C;
    do {
        if (EXIT_SUCCESS != (err = FastHistogram_New(&hist, -5.0f, 2.0f, 7)))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
        for (int i = 0; i < 100; i ++)
        {
            for (int j = 0; j < 100; j++)
            {
                float f = Texture_RangedRandF(range[0], range[1]);
                FastHistogram_Classify(&hist, f);
            }
        }
        int sum = 0;
        int i = 0;
        for (; i < 3; i++)
        {
            sum += hist.counters[i];
        }
        if (sum)
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Texture_RangeRandF() failed.");
        }
        for (; i < 7; i++)
        {
            sum += hist.counters[i];
        }
        if (sum != 10000)
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Texture_RangeRandF() failed.");
        }
        for (; i < hist.counterCount; i++)
        {
            sum += hist.counters[i];
        }
        if (sum != 10000)
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Texture_RangeRandF() failed.");
        }
    } while (0);
    FastHistogram_Delete(&hist);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

int textureUT()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = texture_RangedRandUT()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
        if (EXIT_SUCCESS != (err = texture_RangedRandF_UT()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
        // if (EXIT_SUCCESS != (err = textureBlobUT()))
        // {
        //     LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        // }
        if (EXIT_SUCCESS != (err = textureBlob2UT()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
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
