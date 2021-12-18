#ifndef TEXTURE_H_
#define TEXTURE_H_
#include "ZnccC/ImageC.h"
#include "Pspc/Pcamera.h"
#ifdef __cplusplus
#include <cstdint>
extern "C" {
#else
#include <stdint.h>
#endif
/**
 * @brief texture API consists of
 * void PixelMap_fill() : fill subregion with a selected pattern,
 * int PixelMap_fillstd() : fill with a selected standard pattern,
 * int TextureInterpolator_init() : initialize an interpolator,
 * int TextureInterpolator_get() : get an interpolated value at a point,
 * void TextureInterpolator_delete() : release memory resource allocated to the interpolator
 */
typedef enum {
    PixelFillPattern0,
    PixelFillPattern1,
    PixelFillPattern2,
    PixelFillPattern3,
    PixelFillPatternSquare1Circle0,
    PixelFillPatternSquare1Square0,
    PixelFillPatternSquare0Circle1,
    PixelFillPatternSquare0Square1
} PixelFillPattern_t;

typedef struct {
    RectC_t roi; // x0, y0, width, height
    float filler[4]; // pixel value
    PixelFillPattern_t fillPattern;
} PixelOperation_t, *pPixelOperation_t;
typedef const PixelOperation_t *pcPixelOperation_t;

/**
 * @brief fill inside op->roi with a pattern selected by op->fillPattern
 * 
 * @param pixels [in,out]
 * @param op [in]
 */
void PixelMap_fill(pImageC_t pixels, pcPixelOperation_t op);

typedef enum {
    StdTex0, // square mosaic + corner position marks (100 x 16) x (100 x 16) size
            // bright level 200, dark level 50, grayscale
    StdTex1 // not defined
} StdTex_t;
/**
 * @brief fill standard texture pattern
 * 
 * @param texture [in,out]
 * @param std [in] standard texture number
 */
int PixelMap_fillstd(pImageC_t texture, StdTex_t std);

typedef struct {
    pcImageC_t texture; // source texture pointer
    float* table; // texture interpolator table, each cell consists of four floats, [a0, a1, a2, a3]
        // cell count is 1-less both in width and in height.
    int size[2];
} TextureInterpolator_t, *pTextureInterpolator_t;
typedef const TextureInterpolator_t *pcTextureInterpolator_t;
#define NULLTEXTUREINTERPOLATOR_C   { (pcImageC_t)NULL, (float*)NULL, { 0, 0 } }

/**
 * @brief initialize a texture interpolator
 * 
 * @param interpolator [out]
 * @param texture [in]
 * @return int unix errno compatible code
 */
int TextureInterpolator_init(pTextureInterpolator_t interpolator, pcImageC_t texture);

/**
 * @brief get a texture value at xy which is a normalized coordinate [0..1.0, 0..1.0)
 * 
 * @param interpolator 
 * @param xy 
 * @param value 
 * @return int 
 */
int TextureInterpolator_get(pcTextureInterpolator_t interpolator, const float* xy, float* value);

/**
 * @brief release allocated memory block
 * 
 * @param interpolator 
 */
void TextureInterpolator_delete(pTextureInterpolator_t interpolator);
#ifdef __cplusplus
}
#endif
#endif /* TEXTURE_H_ */