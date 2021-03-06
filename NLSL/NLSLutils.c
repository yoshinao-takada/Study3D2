#include "NLSL/NLSLutils.h"
#include <memory.h>
#include <math.h>

void NLSLutils_addfloats(int size, float* dst, const float* src)
{
    for (int i = 0; i < size; i++)
    {
        dst[i] += src[i];
    }
}

void NLSLutils_subfloats(int size, float* dst, const float* src)
{
    for (int i = 0; i < size; i++)
    {
        dst[i] -= src[i];
    }
}

void NLSLutils_multfloatsscalar(int size, float* dst, float scalar)
{
    for (int i = 0; i < size; i++)
    {
        dst[i] *= scalar;
    }
}

float NLSLutils_dot(int size, const float* v0, const float* v1)
{
    float dot = 0.0f;
    for (int i = 0; i < size; i++)
    {
        dot += v0[i] * v1[i];
    }
    return dot;
}

void NLSLutils_tranformvertex(int size, const float* centroid, const float* worst, float rho, float* transformed)
{
    for (int i = 0; i < size; i++)
    {
        transformed[i] = centroid[i] + rho * (centroid[i] - worst[i]);
    }
}

int NLSLutils_iscloserthan(int size, const float* x0, const float* x1, const float* xth)
{
    int result = 1;
    do {
        for (int i = 0; i < size; i++)
        {
            if (fabsf(x0[i] - x1[i]) >= xth[i])
            { // exceeds threshold
                result = 0;
                break;
            }
        }
    } while (0);
    return result;
}