#include "Pspc/Pvector.h"
#include "NLSL/NLSLutils.h"
#include <math.h>
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

const float* P3V_add(const float* v0, const float* v1, float* vwork)
{
    vwork[0] = v0[0] * v1[3] + v1[0] * v0[3];
    vwork[1] = v0[1] * v1[3] + v1[1] * v0[3];
    vwork[2] = v0[2] * v1[3] + v1[2] * v0[3];
    vwork[3] = v0[3] * v1[3];
    return vwork;
}

const float* P3V_sub(const float* v0, const float* v1, float* vwork)
{
    vwork[0] = v0[0] * v1[3] - v1[0] * v0[3];
    vwork[1] = v0[1] * v1[3] - v1[1] * v0[3];
    vwork[2] = v0[2] * v1[3] - v1[2] * v0[3];
    vwork[3] = v0[3] * v1[3];
    return vwork;
}

const float* P2V_add(const float* v0, const float* v1, float* vwork)
{
    vwork[0] = v0[0] * v1[2] + v1[0] * v0[2];
    vwork[1] = v0[1] * v1[2] + v1[1] * v0[2];
    vwork[2] = v0[2] * v1[2];
    return vwork;
}

const float* P2V_sub(const float* v0, const float* v1, float* vwork)
{
    vwork[0] = v0[0] * v1[2] - v1[0] * v0[2];
    vwork[1] = v0[1] * v1[2] - v1[1] * v0[2];
    vwork[2] = v0[2] * v1[2];
    return vwork;
}

float P3V_dot(const float* v0, const float* v1)
{
    return (v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2]) / (v0[3] * v1[3]);
}

const float* P3V_cross(const float* v0, const float* v1, float* vwork)
{
    vwork[0] = v0[1] * v1[2] - v0[2] * v1[1];
    vwork[1] = v0[2] * v1[0] - v0[0] * v1[2];
    vwork[2] = v0[0] * v1[1] - v0[1] * v1[0];
    vwork[3] = v0[3] * v1[3];
    return vwork;
}

float R3V_azimuth(const float* v)
{
    return atan2f(v[0], v[2]);
}

float R3V_polar(const float* v)
{
    return atan2f(sqrtf(v[0]*v[0] + v[2]*v[2]), v[1]);
}

const float* P3V_toR3V(const float* v, float* vwork)
{
    vwork[0] = v[0]/v[3];
    vwork[1] = v[1]/v[3];
    vwork[2] = v[2]/v[3];
    return vwork;
}

const float* P2V_toR2V(const float* v, float* vwork)
{
    vwork[0] = v[0]/v[2];
    vwork[1] = v[1]/v[2];
    return vwork;
}

const float* P3V_normalize(const float* v, float* vwork)
{
    float rcpLength = fabsf(v[3]) / sqrtf(NLSLutils_dot(3, v, v));
    vwork[0] = v[0] * rcpLength;
    vwork[1] = v[1] * rcpLength;
    vwork[2] = v[2] * rcpLength;
    vwork[3] = v[3];
    return vwork;
}

const float* P3V_orthogonalintersection(const float* p0, const float* dir, const float* p1, float* vwork)
{
    float p[P3VSIZE];
    float squareD = P3V_dot(dir, dir);
    const float* p1_p0 = P3V_sub(p1, p0, p);
    float a = P3V_dot(p1_p0, dir) / squareD;
    vwork[0] = p0[0]/p0[3] + a * dir[0]/dir[3];
    vwork[1] = p0[1]/p0[3] + a * dir[1]/dir[3];
    vwork[2] = p0[2]/p0[3] + a * dir[2]/dir[3];
    vwork[3] = 1.0f;
    vwork[4] = p1[0]/p1[3] - vwork[0];
    vwork[5] = p1[1]/p1[3] - vwork[1];
    vwork[6] = p1[2]/p1[3] - vwork[2];
    vwork[7] = 1.0f;
    return vwork;
}

const float* (*P3Vline_orthogonalintersection)(const float* p0, const float* dir, const float* p1, float* vwork) =
    P3V_orthogonalintersection;

const float* P3Vplane_orthogonalintersection(const float* p0, const float* normaldir, const float* p1, float* vwork)
{
    float p1p0[4];
    float p1p0dotN = P3V_dot(P3V_sub(p1, p0, p1p0), normaldir);
    float scaledNormal[4];
    memcpy(scaledNormal, normaldir, P3VSIZE * sizeof(float));
    NLSLutils_multfloatsscalar(3, scaledNormal, p1p0dotN);
    return P3V_sub(p1, scaledNormal, vwork);
}

const float* P3Vplane_line_intersection
(const float* p0plane, const float* normaldir, const float* p0line, const float* dir, float* vwork)
{
    float p0planep0line[4];
    float p0planep0linedotN = P3V_dot(P3V_sub(p0plane, p0line, p0planep0line), normaldir);
    float DdotN = P3V_dot(dir, normaldir);
    float scaledDir[4];
    memcpy(scaledDir, dir, P3VSIZE * sizeof(float));
    NLSLutils_multfloatsscalar(3, scaledDir, (p0planep0linedotN / DdotN));
    return P3V_add(p0line, scaledDir, vwork);
}

const float* P3Vtriangle_line_intersection
(const float* vertices, const int* indices, const float* p0line, const float* dir, float* vwork)
{
    // retrieve three vertex coordinates
    const float *P_T0 = &vertices[indices[0] * P3VSIZE];
    const float *P_T1 = &vertices[indices[1] * P3VSIZE];
    const float *P_T2 = &vertices[indices[2] * P3VSIZE];

    float Vwork[6 * P3VSIZE];
    const float* normal = P3V_cross(
        P3V_sub(P_T1, P_T0, Vwork),
        P3V_sub(P_T2, P_T0, Vwork + P3VSIZE),
        Vwork + 2 * P3VSIZE); // plane normal vector
    const float* intersection = P3Vplane_line_intersection(P_T0, normal, p0line, dir, vwork);
    const float* v0 = P3V_sub(P_T0, intersection, Vwork);
    const float* v1 = P3V_sub(P_T1, intersection, Vwork + P3VSIZE);
    const float* v2 = P3V_sub(P_T2, intersection, Vwork + 2 * P3VSIZE);
    const float* vx01 = P3V_cross(v0, v1, Vwork + 3 * P3VSIZE);
    const float* vx12 = P3V_cross(v1, v2, Vwork + 4 * P3VSIZE);
    const float* vx20 = P3V_cross(v2, v0, Vwork + 5 * P3VSIZE);
    int sameDirection =
        (P3V_dot(vx01, vx12) > 0.0f) && (P3V_dot(vx12, vx20) > 0.0f) &&  (P3V_dot(vx20, vx01) > 0.0f);
    return sameDirection ? intersection : NULL;
}
