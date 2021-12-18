#ifndef IMAGELOGC_H_
#define IMAGELOGC_H_
#include <ZnccC/ImageC.h>
#include <ZnccC/CostMapC.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef uint8_t (*FPIXTOU8)(float pixelValue);
uint8_t FuncF2U8(float pixelValue);

typedef uint8_t (*FPIXTOU8RANGED)(float pixelValue, float range0, float range1);
uint8_t FuncF2U8Ranged(float pixelValue, float range0, float range1);

int ImageLog_SaveImageC(const char* dir, const char* filename, pcImageC_t image, const FPIXTOU8 pixconv);

int ImageLog_SaveColorMapC(const char* dir, const char* filename, pcImageC_t image, const FPIXTOU8RANGED pixconv, float range0, float range1);

int ImageLog_SaveCSVC(const char* dir, const char* filename, pcImageC_t image);

void ImageLog_SaveMatrixImageCSVC(const char* dir, const char* filename, pcImageC_t image);

void ImageLog_SaveCostSearchTableCSVC(const char* dir, const char* filename, pcCostSearchTableC_t cst);

void ImageLog_ShowImage(pcImageC_t image);

void ImageLog_ShowImageRange(pcImageC_t image, float range0, float range1);

void ImageLog_ShowColorMapC(pcImageC_t image, float range0, float range1);

#ifdef __cplusplus
}
#endif
#endif /* IMAGELOG_H_ */