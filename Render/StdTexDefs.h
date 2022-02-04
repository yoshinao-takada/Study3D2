#ifndef STDTEXDEFS_H_
#define STDTEXDEFS_H_
#ifdef __cplusplus
#include <cstdint>
#include <cstdlib>
extern "C" {
#else
#include <stdint.h>
#include <stdlib.h>
#endif


typedef struct {
    float luminances[2]; // [0]: background, [1]:foreground
    int blockEdge; // block edge length
    int edgeMargin; // edge margin width
    int blockCount[2]; // x-, y- direction block count
} StdRectTex_t, *pStdRectTex_t;
typedef const StdRectTex_t *pcStdRectTex_t;

#define STDRECTTEX0 { \
    { 0.8f, 0.5f }, 100, 20, { 8, 8 } \
}

#define StdRectTex_width(s) ((s)->blockEdge * (s)->blockCount[0] + 2 * (s)->edgeMargin)
#define StdRectTex_height(s) ((s)->blockEdge * (s)->blockCount[1] + 2 * (s)->edgeMargin)
#define StdRectTex_lumU8(s,index) (uint8_t)((s)->luminances[index] * 255.0f)
#define StdRectTex_roi(s,ix,iy) { \
    (s)->edgeMargin + (ix)*(s)->blockEdge, (s)->edgeMargin + (iy)*(s)->blockEdge, \
    (s)->blockEdge, (s)->blockEdge }


typedef struct {
    // [0]: background darkest limit , [1]: background brightest limit, [2]: blob darkest limit, [3]: blob brightest limit
    float luminances[4];
    int wh[2]; // width and height
    
    // box filter kernel size bluring blob 0,1,2,... (2 * halfKSize + 1) is an actual K-size of box filter.
    int halfKSize;

     // [0]: radius base number, [1]: radius random variation,
     // actual radius is defined by blobRadius[0] + rangedRand(-blobRandius[1], blobRaidus[1])
    int blobRadius[2];


    // [0]: blob count base number, [1]: blob count random variation,
    // actual blob count is defined by blobCount[0] + rangedRand(-blobCount[1], blobCount[1])
    int blobCount[2];

    // Note: rangedRand(n0, n1) is a uniform random integer distributed from n0 to n1.

    int blockCount[2];
} StdBlobRectTex_t, *pStdBlobRectTex_t;
typedef const StdBlobRectTex_t *pcStdBlobRectTex_t;

#define STDBLOBRECTTEX0 { \
    { 0.2f, 0.5f, 0.5f, 0.8f }, \
    { 40, 400 }, 5, { 15, 5 }, { 10, 5 }, { 20, 2 }  \
}

/**
 * @brief uniform random number distributed from n0 to n1
 * 
 * @param n0 [in] lower limit of uniform distribution
 * @param n1 [in] upper limit of uniform distribution
 * @return int 
 */
#define Texture_RangedRand(n0,n1) ((n0) + (rand() % ((n1) - (n0))))

#define Texture_R0MAX   ((float)RAND_MAX)

#define Texture_RangedRandF(f0,f1) ((((float)rand())/Texture_R0MAX) * ((f1) - (f0)) + (f0))

#ifdef __cplusplus
}
#endif
#endif /* STDTEXDEFS_H_ */