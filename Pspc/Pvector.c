#include "Pspc/Pvector.h"
#include "NLSL/NLSLutils.h"
#include <math.h>


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