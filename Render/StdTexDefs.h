#ifndef STDTEXDEFS_H_
#define STDTEXDEFS_H_
#ifdef __cplusplus
#include <cstdint>
extern "C" {
#else
#include <stdint.h>
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

#ifdef __cplusplus
}
#endif
#endif /* STDTEXDEFS_H_ */