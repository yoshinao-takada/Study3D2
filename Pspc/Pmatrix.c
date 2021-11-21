#include "Pspc/Pmatrix.h"
#include "Pspc/Pvector.h"
#include "NLSL/NLSLutils.h"
#include "NLSL/NLSLmatrix.h"
#include <math.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

static const int P3Mrows = 4;
static const int P3Mcolumns = 4;
static const int P3Msize = P3Mrows * P3Mrows;
static const int P2Mrows = 3;
static const int P2Mcolumns = 3;
static const int P2Msize = P2Mrows * P2Mrows;

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
    float elTo = R3V_elevation(toP3);
    float elFrom = R3V_elevation(fromP3);
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

const float* P3M_rotatewithaxis(float angle, const float* p0, const float* dir, float* mwork)
{
    const float* result = NULL;
    float* p = NULL;
    int selectedAxis = (int)AxisUndefined;
    float maxDotProduct = 0.0f;
    for (int axisIndex = 0; axisIndex != NLSL_ARRAYSIZE(CoordAxes); axisIndex++)
    {
        float dotProduct = P3V_dot(CoordAxes[axisIndex], dir);
        if (dotProduct > maxDotProduct)
        {
            maxDotProduct = dotProduct;
            selectedAxis = axisIndex;
        }
    }
    p = (float*)calloc(4 * P3Msize, sizeof(float));
    const float* moveP0 = P3M_transport(p0, OriginP3, p); // transport matrix from p0 to origin
    const float* alignDir = P3M_align(dir, CoordAxes[selectedAxis], p + P3Msize);
    const float* toCoordAxis = P3M_mult(alignDir, moveP0, p + 2 * P3Msize);
    const float* toCoordAxisInv = P3M_inv(toCoordAxis, p); // p was recycled.
    const float* rotator = Rotators[selectedAxis](angle, p + P3Msize); // p + P3Msize was recycled.
    result = P3M_mult(toCoordAxisInv, P3M_mult(rotator, toCoordAxis, p + 3 * P3Msize), mwork);
    free(p);
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
    const float* m1 = P3M_rotatewithaxis(cameraposition->rotAng, cameraposition->position, lookAtVector, pmat + P3Msize);

    // move camera origin to look-from position as m2
    const float* m2 = P3M_transport(OriginP3, cameraposition->position, pmat + 2 * P3Msize);

    // calc inv(m2 * m1 * m0)
    const float* mCameraMove = P3M_mult(m2, P3M_mult(m1, m0, pmat + 3 * P3Msize), pmat + 4 * P3Msize);
    result = P3M_inv(mCameraMove, mwork);

    free(pmat);
    return result;
}

static float vpwidth(pcP3MCamera_t camera) {  return (camera->vpTR[0] - camera->vpBL[0]); }
static float vpheight(pcP3MCamera_t camera) {  return (camera->vpTR[1] - camera->vpBL[1]); }
static float vpwh(pcP3MCamera_t camera) { return vpwidth(camera) / vpheight(camera); }
static float vphw(pcP3MCamera_t camera) { return vpheight(camera) / vpwidth(camera); }
static float fovx(pcP3MCamera_t camera) { return camera->fov * ((camera->fovaxis == FOVaxis_x) ? 1.0f : vpwh(camera)); }
static float fovy(pcP3MCamera_t camera) { return camera->fov * ((camera->fovaxis == FOVaxis_y) ? 1.0f : vphw(camera)); }
static float half_fovx(pcP3MCamera_t camera) { return 0.5f * fovx(camera); }
static float half_fovy(pcP3MCamera_t camera) { return 0.5f * fovy(camera); }

const float* P3M_cameraintrinsics(pcP3MCamera_t camera, float* mwork)
{
    NLSL_FILLFLOATS(mwork, 0.0f, P3Mrows * P2Mcolumns);
    float xpe = tanf(half_fovx(camera));
    float xrd = xpe / (0.5f * vpwidth(camera));
    float ype = tanf(half_fovy(camera));
    float yrd = ype / (0.5f * vpheight(camera));
    mwork[0] = 1.0f / xrd;
    mwork[2] = vpwidth(camera) / 2.0f;
    mwork[5] = 1.0f / yrd;
    mwork[6] = vpheight(camera) / 2.0f;
    mwork[10] = 1.0f;
    return mwork;
}