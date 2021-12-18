#include "Render/texture.h"
#include "Render/StdTexDefs.h"
#include "NLSL/NLSLutils.h"
#include <stdio.h>
#include <errno.h>
#include <math.h>

static void PixelMap_fillSolid(pImageC_t pixels, const RectC_t roi, float filler)
{
    float* ptr = pixels->elements + roi[0] + roi[1] * pixels->size[0];
    for (int row = 0; row < roi[3]; row++)
    {
        for (int column = 0; column != roi[2]; column++)
        {
            ptr[column] = filler;
        }
        ptr += pixels->size[0];
    }
}

static void PixelMap_fillSquare(pImageC_t pixels, const RectC_t roi, float fillerOuter, float fillerInner)
{
    const int xrange[2] = { roi[2] / 4, 3 * roi[2] / 4 };
    const int yrange[2] = { roi[3] / 4, 3 * roi[3] / 4 };
    float* ptr = pixels->elements + roi[0] + roi[1] * pixels->size[0];
    for (int row = 0; row != roi[3]; row++)
    {
        for (int column = 0; column != roi[2]; column++)
        {
            ptr[column] =
                ((xrange[0] <= column) && (column < xrange[1]) &&
                 (yrange[0] <= row) && (row < yrange[1])) ? fillerInner : fillerOuter;
        }
        ptr += pixels->size[0];
    }
}

static void PixelMap_fillSquareCircle(pImageC_t pixels, const RectC_t roi, float fillOuter, float fillInner)
{
    const float center[] = { roi[2] * 0.5f, roi[3] * 0.5f };
    const float Rscasle[] = { 2.0f / center[0], 2.0f / center[1] };
    float* ptr = pixels->elements + roi[0] + roi[1] * pixels->size[0];
    for (int row = 0; row != roi[3]; row++)
    {
        for (int column = 0; column != roi[2]; column++)
        {
            float xy[] = { Rscasle[0] * (column - center[0]), Rscasle[1] * (row - center[1]) };
            float sqR = xy[0] * xy[0] + xy[1] * xy[1];
            ptr[column] = (sqR < 1.0f) ? fillInner : fillOuter;
        }
        ptr += pixels->size[0];
    }    
}

void PixelMap_fill(pImageC_t pixels, pcPixelOperation_t op)
{
    if (op->fillPattern == PixelFillPattern0 || op->fillPattern == PixelFillPattern1 ||
        op->fillPattern == PixelFillPattern2 || op->fillPattern == PixelFillPattern3)
    {
        PixelMap_fillSolid(pixels, op->roi, op->filler[op->fillPattern]);
    }
    else if (op->fillPattern == PixelFillPatternSquare1Circle0)
    {
        PixelMap_fillSquareCircle(pixels, op->roi, op->filler[0], op->filler[1]);
    }
    else if (op->fillPattern == PixelFillPatternSquare0Circle1)
    {
        PixelMap_fillSquareCircle(pixels, op->roi, op->filler[1], op->filler[0]);
    }
    else if (op->fillPattern == PixelFillPatternSquare1Square0)
    {
        PixelMap_fillSquare(pixels, op->roi, op->filler[0], op->filler[1]);
    }
    else if (op->fillPattern == PixelFillPatternSquare0Square1)
    {
        PixelMap_fillSquare(pixels, op->roi, op->filler[1], op->filler[0]);
    }
    else
    {
        fprintf(stderr, "Invalid operation, op->fillPattern = %d @ %s, %d\n",
            op->fillPattern, __FILE__, __LINE__);
        abort();
    }
}

PixelFillPattern_t FillPattern(int ix, int iy, int ixEnd, int iyEnd)
{
    int flag = (1 + ix + iy) & 1; // odd:0, even:1
    PixelFillPattern_t fillPattern = (PixelFillPattern_t)flag;
    if (ix == 0 && iy == 0)
    {
        fillPattern = flag ? PixelFillPatternSquare1Circle0 : PixelFillPatternSquare0Circle1;
    }
    else if ((ix == (ixEnd - 1)) && (iy == (iyEnd - 1)))
    {
        fillPattern = flag ? PixelFillPatternSquare1Square0 : PixelFillPatternSquare0Square1;
    }
    return fillPattern;
}

static int PixelMap_fillstd0(pImageC_t texture)
{
    static const StdRectTex_t tex0 = STDRECTTEX0;
    const Size2iC_t size = { StdRectTex_width(&tex0), StdRectTex_height(&tex0) };
    const RectC_t roi = { 0, 0, StdRectTex_width(&tex0), StdRectTex_height(&tex0) };
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = ImageC_New(texture, size, roi)))
        {
            fprintf(stderr, "%s, %d, ImageC_New() failed.\n", __FILE__, __LINE__);
            break;
        }
        // fill background
        NLSL_FILLFLOATS(texture->elements, tex0.luminances[0], texture->size[0]*texture->size[1]);

        // foreach block fill with block pattern
        for (int iy = 0; iy < tex0.blockCount[1]; iy++)
        {
            for (int ix = 0; ix < tex0.blockCount[0]; ix++)
            {
                int isTL = (ix == 0) && (iy == 0);
                int isBR = (ix == (tex0.blockCount[0] - 1)) && (iy == (tex0.blockCount[1]));
                RectC_t blockRoi = StdRectTex_roi(&tex0, ix, iy);
                PixelOperation_t op = 
                {
                    { blockRoi[0], blockRoi[1], blockRoi[2], blockRoi[3] },
                    { tex0.luminances[0], tex0.luminances[1], 0.0f, 0.0f },
                    FillPattern(ix, iy, tex0.blockCount[0], tex0.blockCount[1])
                };
                PixelMap_fill(texture, &op);
            }
        }
    } while (0);
    return err;
}

typedef int (*STDTEXTURECREATOR)(pImageC_t texture);

static const STDTEXTURECREATOR textureCreator[] =
{
    PixelMap_fillstd0
};

int PixelMap_fillstd(pImageC_t texture, StdTex_t std)
{
    return textureCreator[(int)std](texture);
}

int TextureInterpolator_init(pTextureInterpolator_t interpolator, pcImageC_t texture)
{
    int err = EXIT_SUCCESS;
    do {
        interpolator->size[0] = texture->size[0] - 1;
        interpolator->size[1] = texture->size[1] - 1;
        size_t cellCount = interpolator->size[0] * interpolator->size[1];
        size_t floatCount = cellCount * 4;
        interpolator->table = (float*)malloc(floatCount*sizeof(float));
        if (NULL == interpolator->table)
        {
            err = ENOMEM;
            break;
        }
        interpolator->texture = texture;
        float* tablePtr = interpolator->table;
        const float* srcPtr = texture->elements;
        int tableSize[] = { interpolator->size[0], interpolator->size[1] };
        int tableStride = 4 * tableSize[0];
        int srcStride = texture->size[0];
        for (int row = 0; row != texture->size[1]; row++)
        {
            for (int column = 0; column != texture->size[0]; column++)
            {
                float* a = tablePtr + column * 4;
                const float* f = srcPtr + column;
                a[0] = f[0];
                a[1] = -f[0] + f[1];
                a[2] = -f[0] + f[srcStride];
                a[3] = f[0] - f[1] - f[srcStride] + f[srcStride + 1];
            }
            tablePtr += tableStride;
            srcPtr += srcStride;
        }
    } while (0);
    return err;
}

int TextureInterpolator_get(pcTextureInterpolator_t interpolator, const float* xy, float* value)
{
    int err = EXIT_SUCCESS;
    do {
        int ix = (int)floorf(xy[0]);
        int iy = (int)floorf(xy[1]);
        float fracx = xy[0] - (float)ix;
        float fracy = xy[1] - (float)iy;
        const float* a = interpolator->table + 4 * (ix + iy * interpolator->size[0]);
        *value = a[0] + a[1] * fracx + a[2] * fracy + a[3] * fracx * fracy;
    } while (0);
    return err;
}

void TextureInterpolator_delete(pTextureInterpolator_t interpolator)
{
    free((void*)(interpolator->table));
    interpolator->table = (float*)NULL;
}