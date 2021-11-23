#include "Pspc/Pvector.h"
#include "Util/testutils.h"

int PvectorUT();

static const float v0P3[] = { 1.0f, 2.5f, 1.7f, 2.2f };
static const float v0[] = { 1.0f/2.2f, 2.5f/2.2f, 1.7f/2.2f, 1.0f };
static const float v1P3[] = { 1.2f, 2.2f, -1.5f, -2.7f };
static const float v1[] = { -1.2f/2.7f, -2.2f/2.7f, 1.5f/2.7f, 1.0f };
static const float vrsum[] = { 
    1.0f/2.2f - 1.2f/2.7f, 2.5f/2.2f -2.2f/2.7f, 1.7f/2.2f + 1.5f/2.7f, 1.0f
};
static const float vrdiff[] = { 
    1.0f/2.2f + 1.2f/2.7f, 2.5f/2.2f + 2.2f/2.7f, 1.7f/2.2f - 1.5f/2.7f, 1.0f
};
static const float crossv0v1P3[] = { 1.2609f, -0.5960f, 0.1347f, 1.0f };
static const float angleTestVectors[][4] = {
    { 1.0f, 1.0f, 1.0f, 1.5f },
    { 1.0f, -1.0f, 1.0f, 2.5f },
    { -1.0f, 1.0f, 1.0f, 1.5f }
};
static const float testtol = 1.0e-4f;

// testing basic operations
static int Pvector_basicops()
{
    int err = EXIT_SUCCESS;
    float work[4];
    do {
        // add
        const float* testsum = P3V_add(v0P3, v1P3, work);
        if (!TESTEQUALP3V(vrsum, testsum, testtol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stdout, __FUNCTION__, __LINE__, "P3V_add() failed\n");
        }

        // subtract
        const float* testdiff = P3V_sub(v0P3, v1P3, work);
        if (!TESTEQUALP3V(vrdiff, testdiff, testtol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stdout, __FUNCTION__, __LINE__, "P3V_sub() failed.\n");
        }

        // dotproduct
        const float testdot = P3V_dot(v0P3, v1P3);
        const float refdot = v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2];
        if (!TESTEQUALF(refdot, testdot, testtol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stdout, __FUNCTION__, __LINE__, "P3V_dot() failed.\n");
        }

        // crossproduct
        const float* testcross = P3V_cross(v0P3, v1P3, work);
        if (!TESTEQUALP3V(crossv0v1P3, testcross, testtol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stdout, __FUNCTION__, __LINE__, "P3V_cross() failed.\n");
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

// testing angle functions
static int Pvector_angle()
{
    const float refazimuth = atan2f(1.0f, 1.0f);
    const float refpolar = atan2f(sqrtf(2.0f), 1.0f);
    const float pi = 4.0f * refazimuth;
    int err = EXIT_SUCCESS;
    do {
        float az = R3V_azimuth(angleTestVectors[0]);
        if (!TESTEQUALF(refazimuth, az, testtol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "R3V_azimuth() failed, ref=%f, tested=%f\n",
                refazimuth, az);
        }
        az = R3V_azimuth(angleTestVectors[1]);
        if (!TESTEQUALF(refazimuth, az, testtol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "R3V_azimuth() failed, ref=%f, tested=%f\n",
                refazimuth, az);
        }
        az = R3V_azimuth(angleTestVectors[2]);
        if (!TESTEQUALF(-refazimuth, az, testtol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "R3V_azimuth() failed, ref=%f, tested=%f\n",
                -refazimuth, az);
        }
        float el = R3V_polar(angleTestVectors[0]);
        if (!TESTEQUALF(refpolar, el, testtol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "R3V_polar() failed, ref=%f, tested=%f\n",
                refpolar, el);
        }
        el = R3V_polar(angleTestVectors[1]);
        if (!TESTEQUALF(pi - refpolar, el, testtol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "R3V_polar() failed, ref=%f, tested=%f\n",
                pi - refpolar, el);
        }
        el = R3V_polar(angleTestVectors[2]);
        if (!TESTEQUALF(refpolar, el, testtol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "R3V_polar() failed, ref=%f, tested=%f\n",
                refpolar, el);
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

// testing projection related functions
static int Pvector_proj()
{
    int err = EXIT_SUCCESS;
    float work[4];
    do {
        const float azref = R3V_azimuth(v0P3);
        const float plref = R3V_polar(v0P3);
        const float* normalized = P3V_normalize(v0P3, work);
        const float aztest = R3V_azimuth(normalized);
        const float pltest = R3V_polar(normalized);
        const float dottest = P3V_dot(normalized, normalized);
        if (!TESTEQUALF(azref, aztest, testtol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "P3V_normalize() failed, azref=%f, aztest=%f",
                azref, aztest);
        }
        if (!TESTEQUALF(plref, pltest, testtol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "P3V_normalize() failed, plref=%f, pltest=%f",
                plref, pltest);
        }
        if (!TESTEQUALF(1.0f, dottest, testtol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "P3V_normalize() failed, dottest=%f", dottest);
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

static int Pvector_orthogonalintersection()
{
    static const float p0[] = { 1.0f, -1.5f, 2.0f, 0.3f };
    static const float dir[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    static const float p1[] = { -1.0f, -1.0f, 2.0f, 2.0f };
    float work[P3VSIZE * 2];
    float work2[P3VSIZE];
    int err = EXIT_SUCCESS;
    do {
        const float* orthline = P3V_orthogonalintersection(p0, dir, p1, work);
        // check orthogonality of two lines
        float dirDotOrthDir = P3V_dot(dir, orthline + 4);
        if (dirDotOrthDir > testtol)
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "P3V_orthogonalintersection() failed.");
        }
        // check if orthline P0 is on the refline.
        const float* orthlineP0_reflineP0 = P3V_sub(orthline, p0, work2);
        float orthlineP0_reflineP0_dot_reflinedir = P3V_dot(orthlineP0_reflineP0, dir);
        float lengthProduct = sqrtf(
            P3V_dot(orthlineP0_reflineP0, orthlineP0_reflineP0) *
            P3V_dot(dir, dir));
        if (TESTEQUALF(lengthProduct, orthlineP0_reflineP0_dot_reflinedir, testtol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "P3V_")
        }
    } while (0);
    return err;
}

int PvectorUT()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = Pvector_basicops()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Fail in Pvector_basicops()");
        }
        if (EXIT_SUCCESS != (err = Pvector_angle()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Fail in Pvector_angle()");
        }
        if (EXIT_SUCCESS != (err = Pvector_proj()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Fail in Pvector_proj()");
        }
        if (EXIT_SUCCESS != (err = Pvector_orthogonalintersection()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Fail in Pvector_proj()");
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}