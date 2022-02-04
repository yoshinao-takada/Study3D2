#include "Util/imagelogC.h"
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cerrno>
#include <sys/types.h>
#include <sys/stat.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

uint8_t FuncF2U8(float pixelValue)
{
    int32_t i32Value = (int32_t)pixelValue;
    return MIN(255, MAX(0, i32Value));
}

uint8_t FuncF2U8Ranged(float pixelValue, float range0, float range1)
{
    float scaledValue = 256.0f * (pixelValue - range0) / (range1 - range0);
    return FuncF2U8(scaledValue);
}

static char* MakePath(const char* dir, const char* filename)
{
    size_t allocBytes = strlen(dir) + strlen(filename) + 2;
    char* pathBuffer = (char*)malloc(allocBytes);
    if (!pathBuffer) return pathBuffer;
    const char* dirsep = ('/' == dir[strlen(dir) - 1]) ? "" : "/";
    strcpy(pathBuffer, dir);
    struct stat s;
    if (-1 == stat(pathBuffer, &s))
    {
        if (mkdir(pathBuffer, 0777))
        {
            free(pathBuffer);
            pathBuffer = nullptr;
            return pathBuffer;
        }
    }
    snprintf(pathBuffer, allocBytes, "%s%s%s", dir, dirsep, filename);
    return pathBuffer;
}

struct JustCopy {
    pcImageC_t src;
    const FPIXTOU8 pixconv;

    JustCopy(pcImageC_t src_, FPIXTOU8 pixconv_)
        : src(src_), pixconv(pixconv_) {}
    
    void operator () (uint8_t& pixel, const int* loc) const {
        int stride = src->size[0];
        int linearIndex = loc[1] + loc[0] * stride;
        pixel = pixconv(src->elements[linearIndex]);
    }
};

int ImageLog_SaveImageC(const char* dir, const char* filename, pcImageC_t image, const FPIXTOU8 pixconv)
{
    int err = EXIT_SUCCESS;
    char* pathBuffer = nullptr;
    do {
        if (nullptr == (pathBuffer = MakePath(dir, filename)))
        {
            err = EPERM;
            fprintf(stderr, "Fail in MakePath(%s,%s) @ %s, %d\n", dir, filename, __FILE__, __LINE__);
            break;
        }
        cv::Mat1b cvimage(image->size[1], image->size[0]);
        cvimage.forEach(JustCopy(image, pixconv));
        cv::imwrite(pathBuffer, cvimage);
    } while (0);
    if (pathBuffer) free(pathBuffer);
    return err;
}

struct RangedCopy {
    pcImageC_t src;
    const FPIXTOU8RANGED pixconv;
    const float range0, range1;

    RangedCopy(pcImageC_t src_, FPIXTOU8RANGED pixconv_, float range0_, float range1_)
        : src(src_), pixconv(pixconv_), range0(range0_), range1(range1_) {}
    
    void operator () (uint8_t& pixel, const int* loc) const {
        int stride = src->size[0];
        int linearIndex = loc[1] + loc[0] * stride;
        pixel = pixconv(src->elements[linearIndex], range0, range1);
    }
};

int ImageLog_SaveColorMapC
(const char* dir, const char* filename, pcImageC_t image, const FPIXTOU8RANGED pixconv, float range0, float range1)
{
    int err = EXIT_SUCCESS;
    char* pathBuffer = nullptr;
    do {
        if (nullptr == (pathBuffer = MakePath(dir, filename)))
        {
            err = EPERM;
            fprintf(stderr, "Fail in MakePath(%s,%s) @ %s, %d\n", dir, filename, __FILE__, __LINE__);
            break;
        }
        cv::Mat1b cvimage(image->size[1], image->size[0]);
        cvimage.forEach(RangedCopy(image, FuncF2U8Ranged, range0, range1));
        cv::Mat3b colorimage;
        cv::applyColorMap(cvimage, colorimage, cv::COLORMAP_RAINBOW);
        cv::imwrite(pathBuffer, colorimage);
    } while (0);
    if (pathBuffer) free(pathBuffer);
    return err;    
}

void ImageLog_ShowImage(pcImageC_t image)
{
    cv::Mat1b cvimage(image->size[1], image->size[0]);
    cvimage.forEach(JustCopy(image, FuncF2U8));
    cv::imshow(__FUNCTION__, cvimage);
    cv::waitKey();
}

void ImageLog_ShowImageRange(pcImageC_t image, float range0, float range1)
{
    cv::Mat1b cvimage(image->size[1], image->size[0]);
    cvimage.forEach(RangedCopy(image, FuncF2U8Ranged, range0, range1));
    cv::imshow(__FUNCTION__, cvimage);
    cv::waitKey();
}

void ImageLog_ShowColorMapC(pcImageC_t image, float range0, float range1)
{
    cv::Mat1b cvimage(image->size[1], image->size[0]);
    cvimage.forEach(RangedCopy(image, FuncF2U8Ranged, range0, range1));
    cv::Mat3b colorimage;
    cv::applyColorMap(cvimage, colorimage, cv::COLORMAP_RAINBOW);
    cv::imshow(__FUNCTION__, colorimage);
    cv::waitKey();
}