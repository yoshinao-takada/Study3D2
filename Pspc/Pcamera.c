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

int R2GridHomology_new(int xsize, int ysize, pR2GridHomology_t* ppobj)
{
    int err = EXIT_SUCCESS;
    do {
        int elementcount = xsize * ysize;
        size_t databytes = sizeof(R2PointHomology_t) * elementcount;
        size_t allocbytes = sizeof(R2GridHomology_t) + databytes;
        pR2GridHomology_t p = (pR2GridHomology_t)malloc(allocbytes);
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

pR2PointHomology_t R2GridHomology_point(pR2GridHomology_t grid, int xindex, int yindex)
{
    return grid->points + xindex + yindex * grid->GridSize[0];
}

pcR2PointHomology_t R2GridHomology_cpoint(pcR2GridHomology_t grid, int xindex, int yindex)
{
    return grid->points + xindex + yindex * grid->GridSize[0];
}

