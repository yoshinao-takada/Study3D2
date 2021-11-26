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

int PcameraUT()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = GridHomologyUT()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "GridHomologyUT() failed");
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}