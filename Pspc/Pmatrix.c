#include "Pspc/Pmatrix.h"
#include "Pspc/Pvector.h"
#include "NLSL/NLSLutils.h"
#include "NLSL/NLSLmatrix.h"
#include <math.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

static const int P3Mrows = P3MROWS;
static const int P3Mcolumns = P3MCOLUMNS;
static const int P3Msize = P3MSIZE;
static const int P2Mrows = P2MROWS;
static const int P2Mcolumns = P2MCOLUMNS;
static const int P2Msize = P2MSIZE;

static const float CoordAxes[][4] =
{
    { 1.0f, 0.0f, 0.0f, 1.0f }, // positive X-axis
    { 0.0f, 1.0f, 0.0f, 1.0f }, // positive Y-axis
    { 0.0f, 0.0f, 1.0f, 1.0f }, // positive Z-axis
    { -1.0f, 0.0f, 0.0f, 1.0f }, // negative X-axis
    { 0.0f, -1.0f, 0.0f, 1.0f }, // negative Y-axis
    { 0.0f, 0.0f, -1.0f, 1.0f }, // negative Z-axis
};

static const float OriginP3[] = { 0.0f, 0.0f, 0.0f, 1.0f };

const float* P3M_mult(const float* matA, const float* matB, float* mwork)
{
    float* m = mwork;
    const float* matArowhead = matA;
    for (int row = 0; row != P3Mrows; row++)
    {
        const float* matBcolumnhead = matB;
        for (int column = 0; column != P3Mcolumns; column++)
        {
            *m = 0.0f;
            for (int columnA = 0; columnA != P3Mcolumns; columnA++)
            {
                *m += matArowhead[columnA] * matBcolumnhead[columnA * P3Mcolumns];
            }
            m++;
            matBcolumnhead++;
        }
        matArowhead += P3Mcolumns;
    }
    return mwork;
}

const float* P3MP3V_mult(const float* matA, const float* vecB, float* vwork)
{
    const float* matArow = matA;
    for (int row = 0; row < P3Mrows; row++)
    {
        vwork[row] = 0.0f;
        for (int column = 0; column < P3Mcolumns; column++)
        {
            vwork[row] += matArow[column] * vecB[column];
        }
        matArow += P3Mcolumns;
    }
    return vwork;
}

const float* P3M_transport(const float* fromP3, const float* toP3, float* mwork)
{
    NLSL_FILLFLOATS(mwork, 0.0f, P3Msize); // fill mwork with 0s
    mwork[0] = mwork[5] = mwork[10] = mwork[15] = 1.0f; // set diagonal elements
    mwork[3] = (toP3[0]/toP3[3] - fromP3[0]/fromP3[3]);
    mwork[7] = (toP3[1]/toP3[3] - fromP3[1]/fromP3[3]);
    mwork[11] = (toP3[2]/toP3[3] - fromP3[2]/fromP3[3]);
    return mwork;
}

const float* P3M_rotateX(float angle, float* mwork)
{
    NLSL_FILLFLOATS(mwork, 0.0f, P3Msize); // fill mwork with 0s
    mwork[0] = mwork[15] = 1.0f; // set diagonal elements
    mwork[5] = mwork[10] = cosf(angle);
    mwork[6] = -(mwork[9] = sinf(angle));
    return mwork;
}

const float* P3M_rotateY(float angle, float* mwork)
{
    NLSL_FILLFLOATS(mwork, 0.0f, P3Msize); // fill mwork with 0s
    mwork[5] = mwork[15] = 1.0f; // set diagonal elements
    mwork[0] = mwork[10] = cosf(angle);
    mwork[8] = -(mwork[2] = sinf(angle));
    return mwork;
}

const float* P3M_rotateZ(float angle, float* mwork)
{
    NLSL_FILLFLOATS(mwork, 0.0f, P3Msize); // fill mwork with 0s
    mwork[10] = mwork[15] = 1.0f; // set diagonal elements
    mwork[0] = mwork[5] = cosf(angle);
    mwork[1] = -(mwork[4] = sinf(angle));
    return mwork;
}

static const float* P3M_rotateXneg(float angle, float* mwork)
{
    return P3M_rotateX(-angle, mwork);
}

static const float* P3M_rotateYneg(float angle, float* mwork)
{
    return P3M_rotateY(-angle, mwork);
}

static const float* P3M_rotateZneg(float angle, float* mwork)
{
    return P3M_rotateZ(-angle, mwork);
}

typedef const float* (*ROT_AROUND_COORDAXIS)(float angle, float* mwork);

static const ROT_AROUND_COORDAXIS Rotators[] = 
    { P3M_rotateX, P3M_rotateY, P3M_rotateZ, P3M_rotateXneg, P3M_rotateYneg, P3M_rotateZneg };

const float* P3M_align(const float* fromP3, const float* toP3, float* mwork)
{
    const float* result = NULL;
    // azimuthal angles
    float azTo = R3V_azimuth(toP3);
    float azFrom = R3V_azimuth(fromP3);
    // elevation angles
    float elTo = R3V_polar(toP3);
    float elFrom = R3V_polar(fromP3);
    // create working matrices
    float* p = (float*)calloc(4 * P3Msize, sizeof(float));
    if (!p)
    {
        fprintf(stderr, "%s,%d,fail in calloc()\n", __FILE__, __LINE__);
        exit(1);
    }
    { // combine atomic movement
        const float* fromToXPlane = P3M_rotateY(-azFrom, p);
        const float* inXPlane = P3M_rotateX(elTo - elFrom, p + P3Msize);
        const float* XPlaneToTo = P3M_rotateY(azTo, p + 2 * P3Msize);
        result = P3M_mult(XPlaneToTo, P3M_mult(inXPlane, fromToXPlane, p + 3 * P3Msize), mwork);
    }
    free(p);
    return result;
}

const float* P3M_inv(const float* m, float* mwork)
{
    NLSLmatrix_t m_ = { P3Mrows, P3Mcolumns, {m} };
    NLSLmatrix_t result_ = { P3Mrows, P3Mcolumns, {mwork} };
    NLSLmatrix_inv(&m_, &result_);
    return result_.elements.c;
}

const float* P3M_rotateaboutaxis(float angle, const float* p0, const float* dir, float* mwork)
{
    const float* result = NULL;
    const float* xaxis = CoordAxes[0]; // x-positive axis
    float p[4][P3Msize];
    const float *toOrigin = P3M_transport(p0, OriginP3, p[0]); // transport matrix from p0 to origin
    const float *alignXaxis = P3M_align(dir, xaxis, p[1]);
    const float *toRotationAxis = P3M_mult(alignXaxis, toOrigin, p[2]);
    const float *rotate = P3M_rotateX(angle, p[3]);
    const float *returnToOriginalAxis = P3M_inv(toRotationAxis, p[0]);
    result = P3M_mult(returnToOriginalAxis, P3M_mult(rotate, toRotationAxis, p[1]), mwork);
    return result;
}

const float* P3M_tocameracoord(pcP3MCameraPosition_t cameraposition, float* mwork)
{
    const float* result = NULL;
    const int Nmat = 5;
    const int Nvec = 1;
    float* pmat = (float*)calloc(Nmat * P3Msize + Nvec * P3Mrows, sizeof(float)); // 5 matrices and 2 vectors
    float* pvec = pmat + Nmat * P3Msize;

    // align matrix from negative Z axis to look-at direction vector  as m0
    const float* lookAtVector = P3V_sub(cameraposition->lookat, cameraposition->position, pvec);
    const float* m0 = P3M_align(CoordAxes[(int)AxisZNeg], lookAtVector, pmat);

    // rotate about optical axis as m1
    const float* m1 = P3M_rotateaboutaxis(cameraposition->rotAng, cameraposition->position, lookAtVector, pmat + P3Msize);

    // move camera origin to look-from position as m2
    const float* m2 = P3M_transport(OriginP3, cameraposition->position, pmat + 2 * P3Msize);

    // calc inv(m2 * m1 * m0)
    const float* mCameraMove = P3M_mult(m2, P3M_mult(m1, m0, pmat + 3 * P3Msize), pmat + 4 * P3Msize);
    result = P3M_inv(mCameraMove, mwork);

    free(pmat);
    return result;
}

#pragma region viewport_matrix
const float* P2M_inv(const float* matA, float* mwork)
{
    NLSLmatrix_t matA_ = { P2Mrows, P2Mcolumns, { matA } };
    NLSLmatrix_t invMatA_ = { P2Mrows, P2Mcolumns, { mwork } };
    NLSLmatrix_inv(&matA_, &invMatA_);
    return mwork;
}

const float* P2MP2V_mult(const float* matA, const float* vectorB, float* vwork)
{
    vwork[0] = matA[0] * vectorB[0] + matA[1] * vectorB[1] + matA[2] * vectorB[2];
    vwork[1] = matA[3] * vectorB[0] + matA[4] * vectorB[1] + matA[5] * vectorB[2];
    vwork[2] = matA[6] * vectorB[0] + matA[7] * vectorB[1] + matA[8] * vectorB[2];
    return vwork;
}

const float* P2M_mult(const float* matA, const float* matB, float* mwork)
{
    for (int row = 0; row != P2Mrows; row++)
    {
        const float* matArow = matA + row * P2Mcolumns;
        for (int column = 0; column != P2Mcolumns; column++)
        {
            const float* matBcolumn = matB + column;
            int mworkIndex = row * P2Mcolumns + column;
            mwork[mworkIndex] = 0.0f;
            for (int isum = 0; isum != P2Mcolumns; isum++)
            {
                mwork[mworkIndex] += matArow[isum] * matBcolumn[P2Mcolumns * isum];
            }

        }
    }
    return mwork;
}

const float* P3VP2V_project(const float* mat, const float* vector, float* vwork)
{
    vwork[0] = mat[0] * vector[0] + mat[1] * vector[1] + mat[2] * vector[2] + mat[3] * vector[3];
    vwork[1] = mat[4] * vector[0] + mat[5] * vector[1] + mat[6] * vector[2] + mat[7] * vector[3];
    vwork[2] = mat[8] * vector[0] + mat[9] * vector[1] + mat[10] * vector[2] + mat[11] * vector[3];
    return vwork;
}
#pragma endregion viewport_matrix
