#include "Pspc/Pcamera.h"
#include "Pspc/Pmatrix.h"
#include "Pspc/Pvector.h"
#include "NLSL/NLSLmatrix.h"
#include "NLSL/NLSLutils.h"
#include "Util/testutils.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <memory.h>
#include <malloc.h>

#pragma region GENERAL_HOMOLOGY
#define PJ_TOP_LEFT    -325.0f, -240.0f, -1.0f
#define PJ_TOP_RIGHT    315.0f, -240.0f, -1.0f
#define PJ_BOTTOM_LEFT -320.0f, 230.0f, -1.0f
#define PJ_BOTTOM_RIGHT 310.0f, 230.0f, -1.0f
#define PK_TOP_LEFT     -1.0f, 0.7f, -1.0f
#define PK_TOP_RIGHT    1.0f, 0.7f, -1.0f
#define PK_BOTTOM_LEFT  -1.0f, -0.7f, -1.0f
#define PK_BOTTOM_RIGHT 1.0f, -0.7f, -1.0f
#define HMG_TOP_LEFT    { { PJ_TOP_LEFT }, { PK_TOP_LEFT } }
#define HMG_TOP_RIGHT    { { PJ_TOP_RIGHT }, { PK_TOP_RIGHT } }
#define HMG_BOTTOM_LEFT    { { PJ_BOTTOM_LEFT }, { PK_BOTTOM_LEFT } }
#define HMG_BOTTOM_RIGHT    { { PJ_BOTTOM_RIGHT }, { PK_BOTTOM_RIGHT } }
static const P2PointHomology_t homologySource[] =
{ HMG_TOP_LEFT, HMG_TOP_RIGHT, HMG_BOTTOM_LEFT, HMG_BOTTOM_RIGHT };
static const float PK[] = {
    PK_TOP_LEFT, PK_TOP_RIGHT, PK_BOTTOM_LEFT, PK_BOTTOM_RIGHT };
static const float PJ[] = {
    PJ_TOP_LEFT, PJ_TOP_RIGHT, PJ_BOTTOM_LEFT, PJ_BOTTOM_RIGHT };

static int GridHomologyUT()
{
    static const char* positions[] = { "TOP_LEFT", "TOP_RIGHT", "BOTTOM_LEFT", "BOTTOM_RIGHT" };
    static const char* PpLabels[] = { "PJ0", "PJ1", "PJ2", "PJ3" };
    int err = EXIT_SUCCESS;
    pP2GridHomology_t homologies = NULL;
    float mPmat[P2MSIZE];
    float vRpwork[P2VSIZE];
    do {
        if (EXIT_SUCCESS != (err = P2GridHomology_new(2, 2, &homologies)))
        {
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "Fail in P2GridHomology_new()");
        }
        memcpy(homologies->points, homologySource, sizeof(homologySource));
        const float* pmat = P2GridHomology_homographymatrix(homologies, mPmat);
        P2M_print(stderr, "pmat", pmat);
        for (int i = 0; i < 4; i++)
        {
            float v_[3];
            const float* v = P2MP2V_mult(pmat, PK + i * 3, v_);
            printf("{ %f, %f }\n", v[0]/v[2], v[1]/v[2]);
        }
    } while (0);
    NLSL_SAFEFREE(&homologies);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}
#pragma endregion GENERAL_HOMOLOGY

// homography matrix calculation using a triangle-triangle homology with their centroids
#define EQLT0   10.0f, 0.0f, -1.0f
#define EQLT1   0.0f, 10.0f, -1.0f
#define EQLT2   0.0f, 0.0f, 9.0f
#define EQLT3   0.0f, 0.0f, 0.0f
#define ISRT0   0.0f, 0.0f, 1.0f
#define ISRT1   1.0f, 0.0f, 1.0f
#define ISRT2   0.0f, 1.0f, 1.0f
#define ISRT3   1.0f, 1.0f, 1.0f
#define HMT0    { { ISRT0 }, { EQLT0 } }
#define HMT1    { { ISRT1 }, { EQLT1 } }
#define HMT2    { { ISRT2 }, { EQLT2 } }
#define HMT3    { { ISRT3 }, { EQLT3 } }
static const P2PointHomology_t trianglehomologysrc[] = { HMT0, HMT1, HMT2, HMT3 };
static float PKT[] = { EQLT0, EQLT1, EQLT2, EQLT3, 0.0f, -5.0f, 0.5f };
static const float PJT[] = { ISRT0, ISRT1, ISRT2, ISRT3 };

// set EQLT3
void CompletePKT()
{
    float mwork[P3MSIZE];
    float vwork[P3VSIZE];
    static const float normal[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    static const float rotationCenter[] = { 10.0f, 0.0f, 10.0f, 2.0f };
    static const float torotate[] = { EQLT1, 1.0f };
    const float* mrot = P3M_rotateaboutaxis(4.0f * atanf(1.0f), rotationCenter, normal, mwork);
    const float* newEQLT3 = P3MP3V_mult(mrot, torotate, vwork);
    PKT[9] = newEQLT3[0] / newEQLT3[3];
    PKT[10] = newEQLT3[1] / newEQLT3[3];
    PKT[11] = newEQLT3[2] / newEQLT3[3];
}

static int TriangleAndCentroidUT()
{
    // 3-D space equilateral triangle
    pP2GridHomology_t homologies = NULL;
    int err = EXIT_SUCCESS;
    float mPmat[P2MSIZE];
    float vRpwork[P2VSIZE];
    do {
        CompletePKT();
        if (EXIT_SUCCESS != (err = P2GridHomology_new(4, 1, &homologies)))
        {
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "Fail in P2GridHomology_new()");
        }
        memcpy(homologies->points, trianglehomologysrc, sizeof(trianglehomologysrc));
        homologies->points[3].PK[0] = PKT[9];
        homologies->points[3].PK[1] = PKT[10];
        homologies->points[3].PK[2] = PKT[11];
        const float* pmat = P2GridHomology_homographymatrix(homologies, mPmat);
        P2M_print(stderr, "pmat", pmat);
        for (int i = 0; i < 5; i++)
        {
            float v_[3];
            const float* v = P2MP2V_mult(pmat, PKT + i * 3, v_);
            printf("{ %f, %f }\n", v[0]/v[2], v[1]/v[2]);
        }
    } while (0);
    NLSL_SAFEFREE(&homologies);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

int PcameraUT()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = GridHomologyUT()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "GridHomologyUT() failed");
        }
        if (EXIT_SUCCESS != (err = TriangleAndCentroidUT()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "TriangleAndCentroidUT() failed");
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}