#include "Pspc/gnhomography.h"
#include "NLSL/NLSLgnsolver.h"
#include "NLSL/NLSLmatrix.h"
#include "NLSL/NLSLutils.h"
#include <assert.h>
#include <memory.h>
static void FillLastPoints2D(float pk[4][2])
{
    float pkCenter[] = { 0.5f * (pk[1][0] + pk[2][0]), 0.5f * (pk[1][1] + pk[2][1]) };
    float pkCenter_P0[] = { pkCenter[0] - pk[0][0], pkCenter[1] - pk[0][1] };
    float pk3[] = { pkCenter[0] + pkCenter_P0[0], pkCenter[1] + pkCenter_P0[1] };
    pk[3][0] = pk3[0];
    pk[3][1] = pk3[1];
}

static void FillLastPoints3D(float pj[4][3])
{
    float pjCenter[] = { 0.5f * (pj[1][0] + pj[2][0]), 0.5f * (pj[1][1] + pj[2][1]), 0.5f * (pj[1][2] + pj[2][2]) };
    float pjCenter_P0[] = {
        pjCenter[0] - pj[0][0], pjCenter[1] - pj[0][1], pjCenter[2] - pj[0][2]
    };
    float pj3[] = { pjCenter[0] + pjCenter_P0[0], pjCenter[1] + pjCenter_P0[1], pjCenter[2] + pjCenter_P0[2] };
    pj[3][0] = pj3[0];
    pj[3][1] = pj3[1];
    pj[3][2] = pj3[2];
}

static void ObjectiveParamMatrix(float* m, const float* pk_, const float* pj_)
{
    float pk[4][2] = {
        { pk_[0], pk_[1] }, { pk_[2], pk_[3] }, { pk_[4], pk_[5] }, { 0.0f, 0.0f }
    };
    float pj[4][3] = {
        { pj_[0], pj_[1], pj_[2] }, { pj_[3], pj_[4], pj_[5] }, { pj_[6], pj_[7], pj_[8] }, 
        { 0.0f, 0.0f, 0.0f }
    };
    FillLastPoints2D(pk);
    FillLastPoints3D(pj);
    NLSL_FILLFLOATS(m, 0.0f, 72);
    float* mRowHead = m;
    mRowHead[0] = pj[0][0]; mRowHead[1] = pj[0][1]; mRowHead[2] = pj[0][2];
    mRowHead[6] = -pk[0][0] * pj[0][0]; mRowHead[7] = -pk[0][0] * pj[0][1]; mRowHead[8] = -pk[0][0] * pj[0][2];
    mRowHead += 9;
    mRowHead[3] = pj[0][0]; mRowHead[4] = pj[0][1]; mRowHead[5] = pj[0][2]; 
    mRowHead[6] = -pk[0][1] * pj[0][0]; mRowHead[7] = -pk[0][1] * pj[0][1]; mRowHead[8] = -pk[0][1] * pj[0][2];
    mRowHead += 9;
    mRowHead[0] = pj[1][0]; mRowHead[1] = pj[1][1]; mRowHead[2] = pj[1][2];
    mRowHead[6] = -pk[1][0] * pj[1][0]; mRowHead[7] = -pk[1][0] * pj[1][1]; mRowHead[8] = -pk[1][0] * pj[1][2];
    mRowHead += 9;
    mRowHead[3] = pj[1][0]; mRowHead[4] = pj[1][1]; mRowHead[5] = pj[1][2]; 
    mRowHead[6] = -pk[1][1] * pj[1][0]; mRowHead[7] = -pk[1][1] * pj[1][1]; mRowHead[8] = -pk[1][1] * pj[1][2];
    mRowHead += 9;
    mRowHead[0] = pj[2][0]; mRowHead[1] = pj[2][1]; mRowHead[2] = pj[2][2];
    mRowHead[6] = -pk[2][0] * pj[2][0]; mRowHead[7] = -pk[2][0] * pj[2][1]; mRowHead[8] = -pk[2][0] * pj[2][2];
    mRowHead += 9;
    mRowHead[3] = pj[2][0]; mRowHead[4] = pj[2][1]; mRowHead[5] = pj[2][2]; 
    mRowHead[6] = -pk[2][1] * pj[2][0]; mRowHead[7] = -pk[2][1] * pj[2][1]; mRowHead[8] = -pk[2][1] * pj[2][2];
    mRowHead += 9;
    mRowHead[0] = pj[3][0]; mRowHead[1] = pj[3][1]; mRowHead[2] = pj[3][2];
    mRowHead[6] = -pk[3][0] * pj[3][0]; mRowHead[7] = -pk[3][0] * pj[3][1]; mRowHead[8] = -pk[3][0] * pj[3][2];
    mRowHead += 9;
    mRowHead[3] = pj[3][0]; mRowHead[4] = pj[3][1]; mRowHead[5] = pj[3][2]; 
    mRowHead[6] = -pk[3][1] * pj[3][0]; mRowHead[7] = -pk[3][1] * pj[3][1]; mRowHead[8] = -pk[3][1] * pj[3][2];
    NLSLmatrix_t mat = { 8, 9, { m }};
    NLSLmatrix_t matT = { 9, 8, { m + 72 }};
    NLSLmatrix_transpose(&mat, &matT);
}

#pragma region ObjectiveAndJacobian
/**
 * @brief Objective function
 * 
 * @param cx 
 * @param x 
 * @param cy 
 * @param y 
 * @param cp 
 * @param params 
 * @return int 
 */
static int Objective(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    assert(cp == (72  + 72));
    int err = 0;
    float work[8];
    NLSLmatrix_t m = { 8, 9, { params }};
    NLSLmatrix_t h = { 9, 1, { x }};
    NLSLmatrix_t ht = { 1, 9, { x }};
    NLSLmatrix_t mh = { 8, 1, { y }};
    NLSLmatrix_t hh = { 1, 1, { y + 8 }};
    NLSLmatrix_mult(&m, &h, &mh);
    NLSLmatrix_mult(&ht, &h, &hh);
    hh.elements.v[0] -= 1.0f;
    return err;
}

#pragma region Jacobian
static int J_0(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    const int row = 0;
    int err = 0;
    assert(cp == (72  + 72));
    const float *mT_row = params + 72 + row * 8; // transpose(m), corresponding row
    memcpy(y, mT_row, 8 * sizeof(float));
    y[8] = 2 * x[row];
    return err;
}

static int J_1(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    const int row = 1;
    int err = 0;
    assert(cp == (72  + 72));
    const float *mT_row = params + 72 + row * 8; // transpose(m), corresponding row
    memcpy(y, mT_row, 8 * sizeof(float));
    y[8] = 2 * x[row];
    return err;
}

static int J_2(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    const int row = 2;
    int err = 0;
    assert(cp == (72  + 72));
    const float *mT_row = params + 72 + row * 8; // transpose(m), corresponding row
    memcpy(y, mT_row, 8 * sizeof(float));
    y[8] = 2 * x[row];
    return err;
}

static int J_3(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    const int row = 3;
    int err = 0;
    assert(cp == (72  + 72));
    const float *mT_row = params + 72 + row * 8; // transpose(m), corresponding row
    memcpy(y, mT_row, 8 * sizeof(float));
    y[8] = 2 * x[row];
    return err;
}

static int J_4(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    const int row = 4;
    int err = 0;
    assert(cp == (72  + 72));
    const float *mT_row = params + 72 + row * 8; // transpose(m), corresponding row
    memcpy(y, mT_row, 8 * sizeof(float));
    y[8] = 2 * x[row];
    return err;
}

static int J_5(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    const int row = 5;
    int err = 0;
    assert(cp == (72  + 72));
    const float *mT_row = params + 72 + row * 8; // transpose(m), corresponding row
    memcpy(y, mT_row, 8 * sizeof(float));
    y[8] = 2 * x[row];
    return err;
}

static int J_6(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    const int row = 6;
    int err = 0;
    assert(cp == (72  + 72));
    const float *mT_row = params + 72 + row * 8; // transpose(m), corresponding row
    memcpy(y, mT_row, 8 * sizeof(float));
    y[8] = 2 * x[row];
    return err;
}

static int J_7(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    const int row = 7;
    int err = 0;
    assert(cp == (72  + 72));
    const float *mT_row = params + 72 + row * 8; // transpose(m), corresponding row
    memcpy(y, mT_row, 8 * sizeof(float));
    y[8] = 2 * x[row];
    return err;
}

static int J_8(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    const int row = 8;
    int err = 0;
    assert(cp == (72  + 72));
    const float *mT_row = params + 72 + row * 8; // transpose(m), corresponding row
    memcpy(y, mT_row, 8 * sizeof(float));
    y[8] = 2 * x[row];
    return err;
}

static const NLSL_OBJECTIVE jacobian_colums[] = { J_0, J_1, J_2, J_3, J_4, J_5, J_6, J_7, J_8 };
#pragma endregion Jacobian
#pragma endregion ObjectiveAndJacobian
static const float xInit[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
static const float xEps[] = { 1.0e-5f, 1.0e-5f, 1.0e-5f, 1.0e-5f, 1.0e-5f, 1.0e-5f, 1.0e-5f, 1.0e-5f, 1.0e-5f };
static const float yEps = 1.0e-8f;
const float* gnhomography(const float* pk, const float* pj, float* work)
{
    const float* h = NULL;
    pNLSLgnsolver_t solver = NULL;
    do {
        solver = NLSLgnsolver_new(9, 9, 72 + 72);
        if (!solver)
        {
            break;
        }
        // set Jacobian callbacks
        memcpy(NLSLgnsolver_jacobian(solver), jacobian_colums, sizeof(jacobian_colums));

        // set configuration parameters
        pNLSLsolverconf_t conf = NLSLgnsolver_conf(solver);
        memcpy(conf->xInitial, xInit, sizeof(xInit));
        memcpy(conf->xEps, xEps, sizeof(xEps));
        conf->thObjective = yEps;
        conf->trace = NULL;
        conf->objective = Objective;
        ObjectiveParamMatrix(conf->params, pk, pj);

        // Execute
        NLSLgnsolver_init(solver);
        int err = NLSLgnsolver_exec(solver, 8); // iterative operation
        if (err)
        {
            break;
        }

        // Get results
        memcpy(work, NLSLgnsolver_xfinal(solver), 9 * sizeof(float));
        h = work;
    } while (0);
    NLSLgnsolver_delete(&solver);
    return h;
}