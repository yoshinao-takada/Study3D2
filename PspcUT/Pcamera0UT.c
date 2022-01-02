#include "Pspc/Pcamera0.h"
#include "Util/testutils.h"
#include "Pspc/Pvector.h"
#include "Pspc/Pmatrix.h"
#include "NLSL/NLSLmatrix.h"
#include <stdlib.h>
#include <stdio.h>
#define VPWIDTH 640.0f
#define VPHEIGHT 480.0f
#define VPDIAGONAL 800.0f
#define VPFOCAL
#define WOVD    (VPWIDTH/VPDIAGONAL)
#define HOVD    (VPHEIGHT/VPDIAGONAL)
#define D35 43.26661530556787e-3f
#define FL35 22.0e-3f

static const float tol = 1.0e-2f;

static const Camera35mmConf_t conf0 = 
{
    { VPWIDTH, VPHEIGHT }, // viewport size represented in pixels
    22e-3f // 35 mm equivalent focal length
};

static const float Pwtest[] = 
{
    D35 * WOVD / 2.0f, D35 * HOVD / 2.0f, -FL35, 1.0f,
    -D35 * WOVD, -D35 * HOVD, -2.0f * FL35, 1.0f,
    0.0f, 0.0f, -FL35, 1.0f
};

static const float Prref[] =
{
    640.0f, 0.0f, 1.0f,
    0.0f, 480.0f, 1.0f,
    320.0f, 240.0f, 1.0f
};

static float intrin3x4[12];

static int Pcamera0IntrinsicUT()
{
    int err = EXIT_SUCCESS;
    float Prtest[3];
    do {
        Camera35mmConf_mat(&conf0, intrin3x4);
        P23MP3V_mult(intrin3x4, Pwtest, Prtest);
        if (!TESTEQUALP2V(Prref, Prtest, tol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "fail in point-TR");
        }
        P23MP3V_mult(intrin3x4, Pwtest + P3VSIZE, Prtest);
        if (!TESTEQUALP2V(Prref + P2VSIZE, Prtest, tol))
        {
            err = EXIT_FAILURE;
            P2V_print(stderr, "point-BL", Prtest);
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "fail in point-BL");
        }
        P23MP3V_mult(intrin3x4, Pwtest + 2 * P3VSIZE, Prtest);
        if (!TESTEQUALP2V(Prref + 2 * P2VSIZE, Prtest, tol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "fail in point-Center");
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

static const P3MCameraPosition_t campos0 =
{
    0.0f, // rotation angle
    { -FL35 * VPDIAGONAL / D35, 0.0f, 0.0f, 1.0f }, // camera position
    { 0.0f, 0.0f, 0.0f, 1.0f } // camera look-at point
};

static const float Pvptest[] = 
{
    640.0f, 0.0f,
    0.0f, 480.0f,
    320.0f, 240.0f
};

static const float TargetPlaneP0[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static const float TargetPlaneNormal[] = { -1.0f, 0.0f, 0.0f, 1.0f };

static int Pcamera0ViewlineUT()
{
    int err = EXIT_SUCCESS;
    float p0[P3VSIZE], dir[P3VSIZE], extrinsic[P3MSIZE], projection[P2VSIZE * P3MSIZE],
        intersection[P3VSIZE];
    NLSLmatrix_t intrinsic_ = { P2VSIZE, P3VSIZE, { intrin3x4 }};
    NLSLmatrix_t extrinsic_ = { P3VSIZE, P3VSIZE, { extrinsic }};
    NLSLmatrix_t projection_ = { P2VSIZE, P3VSIZE, { projection }};
    do {
        P3M_tocameracoord(&campos0, extrinsic);
        NLSLmatrix_mult(&intrinsic_, &extrinsic_, &projection_);
        CameraViewline(projection, Pvptest, &campos0, p0, dir);
        P3Vplane_line_intersection(TargetPlaneP0, TargetPlaneNormal, p0, dir, intersection);
        if (!TESTEQUALF(intersection[1]/intersection[3], VPHEIGHT/2.0f, tol) ||
            !TESTEQUALF(intersection[2]/intersection[3], VPWIDTH/2.0f, tol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "Fail in point-TR");
        }
        CameraViewline(projection, Pvptest + R2VSIZE, &campos0, p0, dir);
        P3Vplane_line_intersection(TargetPlaneP0, TargetPlaneNormal, p0, dir, intersection);
        if (!TESTEQUALF(intersection[1]/intersection[3], -VPHEIGHT/2.0f, tol) ||
            !TESTEQUALF(intersection[2]/intersection[3], -VPWIDTH/2.0f, tol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "Fail in point-BL");
        }
        CameraViewline(projection, Pvptest + 2*R2VSIZE, &campos0, p0, dir);
        P3Vplane_line_intersection(TargetPlaneP0, TargetPlaneNormal, p0, dir, intersection);
        if (!TESTEQUALF(intersection[1]/intersection[3], 0.0f, tol) ||
            !TESTEQUALF(intersection[2]/intersection[3], 0.0f, tol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "Fail in point-Center");
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

int Pcamera0UT()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = Pcamera0IntrinsicUT()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
        if (EXIT_SUCCESS != (err = Pcamera0ViewlineUT()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "err = %d", err);
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}