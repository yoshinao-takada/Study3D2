#include "Pspc/homography.h"
#include "Pspc/Pmatrix.h"
#include "Pspc/Pvector.h"
#include "NLSL/NLSLmatrix.h"
#include <math.h>

static const float* P3V_AnotherPoint(const float* const *vert, float* work)
{
    float vwork[P3VSIZE];
    P3V_add(vert[1], vert[2], vwork);
    return P3V_sub(vwork, vert[0], work);
}

static const float* R3V_AnotherPoint(const float* const *vert, float* work)
{
    float vwork[R3VSIZE];
    vwork[0] = vert[1][0] + vert[2][0];
    vwork[1] = vert[1][1] + vert[2][1];
    vwork[2] = vert[1][2] + vert[2][2];
    work[0] = vwork[0] - vert[0][0];
    work[1] = vwork[1] - vert[0][1];
    work[2] = vwork[2] - vert[0][2];
    return work;
}

static const float* P2V_AnotherPoint(const float* const *vert, float* work)
{
    float vwork[P2VSIZE];
    P2V_add(vert[1], vert[2], vwork);
    return P2V_sub(vwork, vert[0], work);
}

static const float* R2V_AnotherPoint(const float* const *vert, float* work)
{
    float vwork[R2VSIZE];
    vwork[0] = vert[1][0] + vert[2][0];
    vwork[1] = vert[1][1] + vert[2][1];
    work[0] = vwork[0] - vert[0][0];
    work[1] = vwork[1] - vert[0][1];
    return work;
}

typedef const float* (*FUNC_VERT)(const float* const *vert, float* work);

/**
 * @brief Calculate symmetrical point of vert[0] about center of vert[1] and vert[2] as the 4th vertex
 * of a parallelogram.
 * 
 * @param triangle 
 * @param work 
 * @return const float* 
 */
const float* P3TriangleHomology_AnotherPoint(pcP3Triangle_t triangle, float* work)
{
    static const FUNC_VERT funcs[] = {
        P3V_AnotherPoint, R3V_AnotherPoint,
        P2V_AnotherPoint, R2V_AnotherPoint
    };
    return funcs[(int)(triangle->type)](triangle->vert, work);
}

static void Set4Vert(float xy[4][2], pcP3Triangle_t triangle, const float* vert3)
{
    if (triangle->type == PxV_R2V)
    {
        xy[0][0] = triangle->vert[0][0];
        xy[0][1] = triangle->vert[0][1];
        xy[1][0] = triangle->vert[1][0];
        xy[1][1] = triangle->vert[1][1];
        xy[2][0] = triangle->vert[2][0];
        xy[2][1] = triangle->vert[2][1];
        xy[3][0] = vert3[0];
        xy[3][1] = vert3[1];
    }
    else
    {
        xy[0][0] = triangle->vert[0][0] / triangle->vert[0][2];
        xy[0][1] = triangle->vert[0][1] / triangle->vert[0][2];
        xy[1][0] = triangle->vert[1][0] / triangle->vert[1][2];
        xy[1][1] = triangle->vert[1][1] / triangle->vert[1][2];
        xy[2][0] = triangle->vert[2][0] / triangle->vert[2][2];
        xy[2][1] = triangle->vert[2][1] / triangle->vert[2][2];
        xy[3][0] = vert3[0] / vert3[2];
        xy[3][1] = vert3[1] / vert3[2];
    }
}

const float* P3TriangleHomology_homographymatrix(pcP3TriangleHomology_t homology, float* work)
{
    float vert3[2][P3VSIZE];
    float pk[4][2], pj[4][2];
    float m_[8 * 8], minv_[8 * 8], v_[8];
    NLSLmatrix_t 
        m = { 8, 8, { m_ } }, minv = { 8, 8, { minv_ } },
        h = { 8, 1, { work } }, v = { 8, 1, { v_ } };
    P3TriangleHomology_AnotherPoint(&homology->src, vert3[0]);
    P3TriangleHomology_AnotherPoint(&homology->dst, vert3[1]);
    Set4Vert(pk, &homology->src, vert3[0]);
    Set4Vert(pj, &homology->dst, vert3[1]);
    for (int i = 0; i < 4; i++)
    {
        float* mrow = m.elements.v + m.columns * 2 * i;
        mrow[0] = pk[i][0];
        mrow[1] = pk[i][1];
        mrow[2] = 1.0f;
        mrow[3] = mrow[4] = mrow[5] = 0.0f;
        mrow[6] = -pj[i][0] * pk[i][0];
        mrow[7] = -pj[i][0] * pk[i][1];
        mrow += m.columns;
        mrow[0] = mrow[1] = mrow[2] = 0.0f;
        mrow[3] = pk[i][0];
        mrow[4] = pk[i][1];
        mrow[5] = 1.0f;
        mrow[6] = -pj[i][1] * pk[i][0];
        mrow[7] = -pj[i][1] * pk[i][1];
    }
    float* vcol = v.elements.v;
    vcol[0] = pj[0][0];
    vcol[1] = pj[0][1];
    vcol[2] = pj[1][0];
    vcol[3] = pj[1][1];
    vcol[4] = pj[2][0];
    vcol[5] = pj[2][1];
    vcol[6] = pj[3][0];
    vcol[7] = pj[3][1];
    NLSLmatrix_inv(&m, &minv);
    NLSLmatrix_mult(&minv, &v, &h);
    work[8] = 1.0f;
    return work;
}