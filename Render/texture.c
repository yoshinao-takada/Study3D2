#include "Render/texture.h"
#include "Render/StdTexDefs.h"
#include "NLSL/NLSLutils.h"
#include "ZnccC/SumKernelC.h"
#include "Util/imagelogC.h"
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <memory.h>

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

#pragma region BlobGeneratorBase
/**
 * @brief set pixels to ranged float numbers around the center of the blob
 * 
 * @param work [in,out] render target
 * @param center [in] pixel coord relative to work->roi orgin
 * @param radius [in] blob radius
 * @param colorRange [in] colorRange[0]: darkest limit, colorRange[1]: brightest limit
 */
static void PixelMap_CreateBlob(pImageC_t work, const Point2iC_t center, int radius, const float* colorRange)
{
    for (int r = 0; r < radius; r++)
    {
        int circumference = 4 * (2 * r + 1) - 4;
        const int maxOffsetStep = 2 * r;
        for (int iTrial = 0; iTrial < circumference; iTrial++)
        {
            Point2iC_t offset = { 0, 0 };
            for (int iOffsetStep = 0; iOffsetStep < maxOffsetStep; iOffsetStep++)
            {
                offset[0] += Texture_RangedRand(-1, 2); // random number -1, 0, or 1
                offset[1] += Texture_RangedRand(-1, 2);
            }
            const Point2iC_t absPoint = { work->roi[0] + center[0] + offset[0], work->roi[1] + center[1] + offset[1] };
            if ((absPoint[0] < 0) || (absPoint[1] < 0) || (work->size[0] <= absPoint[0]) || (work->size[1] <= absPoint[1]))
            { // out of work area
                continue;
            }
            const int linearOffset = absPoint[0] + absPoint[1] * work->size[0];
            work->elements[linearOffset] = Texture_RangedRandF(colorRange[0], colorRange[1]);
        }
    }
}

/**
 * @brief blur source image by a box filter
 * 
 * @param work [in] source image
 * @param halfKSize [in] half of box filter K-size; (halfKSize * 2 + 1 ) = box K-size.
 * @param tex [out] blurred image
 * @return unix errno compatible return code
 */
static void PixelMap_BlurBlob(pImageC_t work, const RectC_t sumRect, pImageC_t tex)
{
    SumKernelC_t sk = MK_SUMKERNEL(work, sumRect);
    int err = EXIT_SUCCESS;
    ImageC_t blurred  = NULLIMAGE_C;
    assert(AREA_RECT(work->roi) == AREA_SIZE(tex->size));
    float* texPtr = tex->elements;
    const float rcpArea = 1.0f / AREA_RECT(sumRect);
    for (int row = 0; row < work->roi[3]; row++)
    {
        for (int col = 0; col < work->roi[2]; col++)
        {
            texPtr[col] = rcpArea * SumKernelC_SATSum(&sk, col);
        }
        texPtr += tex->size[0];
        sk.base += work->size[0];
    }
}

static int PixelMap_SolidAndBlobsBase(pImageC_t tex, pcStdBlobRectTex_t texConf)
{
    int err = EXIT_SUCCESS;
    ImageC_t work = NULLIMAGE_C;
    do {
        int32_t kSize = 2 * texConf->halfKSize  + 1;
        RectC_t BoxK = { -texConf->halfKSize, -texConf->halfKSize, kSize, kSize };
        RectC_t marginRect = { BoxK[0] - 2, BoxK[1] - 2, BoxK[2] + 4, BoxK[3] + 4 };
        Size2iC_t workSize = { texConf->wh[0] + marginRect[2], texConf->wh[1] + marginRect[3] };
        RectC_t workRoi = { -marginRect[0], -marginRect[1], texConf->wh[0], texConf->wh[1] };
        Size2iC_t texSize = { texConf->wh[0], texConf->wh[1] };
        RectC_t texRoi = { 0, 0, texConf->wh[0], texConf->wh[1] };
        if ((EXIT_SUCCESS != (err = ImageC_New(&work, workSize, workRoi))) ||
            (EXIT_SUCCESS != (err = ImageC_New(tex, texSize, texRoi))))
        {
            break;
        }
        {
            float backgroundLuminance = Texture_RangedRandF(texConf->luminances[0], texConf->luminances[1]);
            NLSL_FILLFLOATS(work.elements, backgroundLuminance, AREA_SIZE(work.size)); // set background
        }
        const int BlobCount = Texture_RangedRand(
            texConf->blobCount[0] - texConf->blobCount[1], 
            texConf->blobCount[0] + texConf->blobCount[1] + 1);
        // create blobs
        for (int iBlob = 0; iBlob != BlobCount; iBlob++)
        {
            const Point2iC_t center = { Texture_RangedRand(0, texConf->wh[0]), Texture_RangedRand(0, texConf->wh[1]) };
            const int radius = Texture_RangedRand(
                texConf->blobRadius[0] - texConf->blobRadius[1], 
                texConf->blobRadius[0] + texConf->blobRadius[1] + 1);
            PixelMap_CreateBlob(&work, center, radius, texConf->luminances + 2);
        }
        ImageC_Integrate(&work);
        PixelMap_BlurBlob(&work, BoxK, tex);
    } while (0);
    ImageC_Delete(&work);
    if (err)
    {
        ImageC_Delete(tex);
    }
    return err;
}
#pragma endregion BlobGeneratorBase

static void PixelMap_blockcopy(pImageC_t dst, pcImageC_t src, const Point2iC_t dstO)
{
    int dstRow = dstO[1];
    for (int row = 0; row < src->roi[3]; row++, dstRow++)
    {
        if ((dstRow < 0) || (dstRow >= dst->roi[3])) continue;
        const float* ptrSrc = ImageC_BeginC(src) + row * src->size[0];
        float* ptrDst = ImageC_Begin(dst) + dstRow * dst->size[0] + dstO[0];
        memcpy(ptrDst, ptrSrc, src->roi[2] * sizeof(float));
    }
}

static int PixelMap_fillstd1(pImageC_t texture)
{
    static const StdBlobRectTex_t texConf = STDBLOBRECTTEX0;
    int err = EXIT_SUCCESS;
    do {
        { // init texture
            const Size2iC_t WholeTextureSize = { texConf.wh[0] * texConf.blockCount[0], texConf.wh[1] * texConf.blockCount[1] };
            const RectC_t WholeRoi = { 0, 0, WholeTextureSize[0], WholeTextureSize[1] };
            if (EXIT_SUCCESS != (err = ImageC_New(texture, WholeTextureSize, WholeRoi)))
            {
                break;
            }
        }
        for (int iBlockX = 0; iBlockX < texConf.blockCount[0]; iBlockX++)
        {
            for (int iBlockY = 0; iBlockY <= texConf.blockCount[1]; iBlockY++)
            {
                ImageC_t block = NULLIMAGE_C;
                if ((0 == (iBlockX & 1)) && (iBlockY == texConf.blockCount[1])) continue;
                if (EXIT_SUCCESS != (err = PixelMap_SolidAndBlobsBase(&block, &texConf)))
                {
                    ImageC_Delete(&block);
                    break;
                }
                Point2iC_t copyDestOrigin = {
                    iBlockX * texConf.wh[0],
                    iBlockY * texConf.wh[1] - ((iBlockX & 1) ? texConf.wh[1]/2 : 0)
                };
                PixelMap_blockcopy(texture, &block, copyDestOrigin);
                ImageC_Delete(&block);
            }
            if (err) break;
        }
    } while (0);
    if (err)
    {
        ImageC_Delete(texture);
    }
    return err;
}

typedef int (*STDTEXTURECREATOR)(pImageC_t texture);

static const STDTEXTURECREATOR textureCreator[] =
{
    PixelMap_fillstd0,
    PixelMap_fillstd1
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
        interpolator->table = (float*)calloc(floatCount, sizeof(float));
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
        for (int row = 0; row != interpolator->size[1]; row++)
        {
            for (int column = 0; column != interpolator->size[0]; column++)
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
        if ((ix >= interpolator->size[0]) || (iy >= interpolator->size[1]) || (ix < 0) || (iy < 0))
        {
            err = ERANGE;
            break;
        }
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

static const StdBlobRectTex_t blobTexConfs[] = {
    STDBLOBRECTTEX0
};

int PixelMap_SolidAndBlobs(pImageC_t tex, StdTex_t texConf)
{
    assert(texConf == StdTex1);
    return PixelMap_SolidAndBlobsBase(tex, &blobTexConfs[texConf - StdTex1]);
}