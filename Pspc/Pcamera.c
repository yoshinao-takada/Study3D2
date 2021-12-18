#include "Pspc/Pcamera.h"
#include "Pspc/Pmatrix.h"
#include "NLSL/NLSLutils.h"
#include "NLSL/NLSLmatrix.h"
#include <math.h>
#include <errno.h>
#include <malloc.h>
#include <stdlib.h>

float vpwidth(pcP3MCamera_t camera) {  return (camera->vpTR[0] - camera->vpBL[0]); }
float vpheight(pcP3MCamera_t camera) {  return (camera->vpTR[1] - camera->vpBL[1]); }
float vpwh(pcP3MCamera_t camera) { return vpwidth(camera) / vpheight(camera); }
float vphw(pcP3MCamera_t camera) { return vpheight(camera) / vpwidth(camera); }
float fovx(pcP3MCamera_t camera) { return camera->fov * ((camera->fovaxis == FOVaxis_x) ? 1.0f : vpwh(camera)); }
float fovy(pcP3MCamera_t camera) { return camera->fov * ((camera->fovaxis == FOVaxis_y) ? 1.0f : vphw(camera)); }
float half_fovx(pcP3MCamera_t camera) { return 0.5f * fovx(camera); }
float half_fovy(pcP3MCamera_t camera) { return 0.5f * fovy(camera); }

const float* P3M_cameraintrinsics(pcP3MCamera_t camera, float* mwork)
{
    NLSL_FILLFLOATS(mwork, 0.0f, P3MROWS * P2MCOLUMNS);
    float xpe = tanf(half_fovx(camera));
    float xrd = xpe / (0.5f * vpwidth(camera));
    float ype = tanf(half_fovy(camera));
    float yrd = ype / (camera->vpTR[1] - 0.5f * (camera->vpBL[1] + camera->vpTR[1]));
    mwork[0] = 1.0f / xrd;
    mwork[2] = vpwidth(camera) / 2.0f;
    mwork[5] = 1.0f / yrd;
    mwork[6] = vpheight(camera) / 2.0f;
    mwork[10] = 1.0f;
    return mwork;
}

int P2GridHomology_new(int xsize, int ysize, pP2GridHomology_t* ppobj)
{
    int err = EXIT_SUCCESS;
    do {
        int elementcount = xsize * ysize;
        size_t databytes = sizeof(P2PointHomology_t) * elementcount;
        size_t allocbytes = sizeof(P2GridHomology_t) + databytes;
        pP2GridHomology_t p = (*ppobj = (pP2GridHomology_t)malloc(allocbytes));
        if (!p)
        {
            fprintf(stderr, "%s,%d,Fail in malloc(%zu)\n", __FILE__, __LINE__, allocbytes);
            err = ENOMEM;
            break;
        }
        p->GridSize[0] = xsize;
        p->GridSize[1] = ysize;
    } while (0);
    return err;
}

pP2PointHomology_t P2GridHomology_point(pP2GridHomology_t grid, int xindex, int yindex)
{
    return grid->points + xindex + yindex * grid->GridSize[0];
}

pcP2PointHomology_t P2GridHomology_cpoint(pcP2GridHomology_t grid, int xindex, int yindex)
{
    return grid->points + xindex + yindex * grid->GridSize[0];
}

const float* P2GridHomology_homographymatrix(pcP2GridHomology_t grid, float* mwork)
{
    float mLHSwork[8 * 8], vRHSwork[8];
    NLSLmatrix_t
        mLHS = { 8, 8, { mLHSwork } },
        vRHS = { 8, 1, { vRHSwork } },
        mH = { 8, 1, { mwork } };
    float* rowLHS = mLHSwork;
    float* rowRHS = vRHSwork;
    for (int iPoint = 0; iPoint < 4; iPoint++)
    {
        pcP2PointHomology_t homology = &grid->points[iPoint];
        float xJ = homology->PJ[0] / homology->PJ[2];
        float yJ = homology->PJ[1] / homology->PJ[2];
        float xK = homology->PK[0] / homology->PK[2];
        float yK = homology->PK[1] / homology->PK[2];
        rowRHS[0] = xJ;
        rowRHS[1] = yJ;
        rowRHS += 2;
        rowLHS[0] = xK;
        rowLHS[1] = yK;
        rowLHS[2] = 1.0f;
        rowLHS[3] = rowLHS[4] = rowLHS[5] = 0.0f;
        rowLHS[6] = -xJ * xK;
        rowLHS[7] = -xJ * yK;
        rowLHS += 8;
        rowLHS[0] = rowLHS[1] = rowLHS[2] = 0.0f;
        rowLHS[3] = xK;
        rowLHS[4] = yK;
        rowLHS[5] = 1.0f;
        rowLHS[6] = -yJ * xK;
        rowLHS[7] = -yJ * yK;
        rowLHS += 8;
    }
    NLSLmatrix_solve(&mLHS, &vRHS, &mH);
    mwork[8] = 1.0f;
    return mwork;
}