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
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

static int Pvector_plane_orthogonalintersection()
{
    static const float p0[] = { 1.0f, -1.5f, 2.0f, 0.3f };
    static const float normal[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    static const float p1[] = { -1.0f, -1.0f, 2.0f, 2.0f };
    float work[P3VSIZE * 3];
    int err = EXIT_SUCCESS;
    do {
        const float* intersection = P3Vplane_orthogonalintersection(p0, normal, p1, work);
        const float* anotherNormal = P3V_sub(p1, intersection, work + P3VSIZE);
        float normalDotAnotherNormal = P3V_dot(normal, anotherNormal);
        float lengthProduct = sqrtf(P3V_dot(normal, normal) * P3V_dot(anotherNormal, anotherNormal));
        if (!TESTEQUALF(fabsf(normalDotAnotherNormal), lengthProduct, testtol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "P3V_")
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

static int Pvector_point_is_in_plane(const float* point, const float* p0, const float* normal)
{
    float Vwork[P3VSIZE * 4];
    const float* p0_point = P3V_sub(point, p0, Vwork);
    float normalDotP0_point = P3V_dot(normal, p0_point);
    return fabsf(normalDotP0_point) < testtol;
}

static int Pvector_plane_line_intersection()
{
    static const float p0[] = { 1.0f, 2.2f, -1.5f, 2.0f };
    static const float normal[] = { 1.2f, 1.0f, -1.5f, 1.0f };
    static const float p0line[][4] = {
        { 1.5f, -1.5f, -1.5f, 1.0f },
        { 2.2f, 2.5f, -1.0f, 2.0f }
    };
    static const float p0dir[][4] = {
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 2.2f, 2.5f, -1.5f, -2.0f }
    };
    int err = EXIT_SUCCESS;
    float work[P3VSIZE * 3];
    do {
        for (int i = 0; i < 2; i++)
        {
            const float* intersection = P3Vplane_line_intersection(p0, normal, p0line[i], p0dir[i], work);
            if (!Pvector_point_is_in_plane(intersection, p0, normal))
            {
                err = EXIT_FAILURE;
                LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "intersection is not in the plane");
            }
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

static int Pvector_triangle_line_intersection()
{
    static const float vertices[] =
    {
        1.0f, 0.0f, 0.0f, 1.0f, // (1, 0, 0)
        0.0f, 1.0f, 0.0f, 1.0f, // (0, 1, 0)
        0.0f, 0.0f, 1.0f, 1.0f, // (0, 0, 1)
        0.0f, -1.0f, 0.0f, 1.0f // (0, -1, 0)
    };
    static const int indices[][3] =
    {
        { 0, 1, 2 },
        { 2, 3, 0 }
    };
    static const float p0[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    static const float dir0[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    static const float dir1[] = { 1.0f, -1.0f, 1.0f, 1.0f };
    static const float dir2[] = { 1.0f, 0.0f, -0.1f, 1.0f };
    static const float refp0[] = { 1.0f, 1.0f, 1.0f, 3.0f };
    float work[P3VSIZE];
    const float* intersection = NULL;
    int err = EXIT_SUCCESS;
    do {
        intersection = P3Vtriangle_line_intersection(vertices, indices[0], p0, dir0, work);
        if (!Pvector_point_is_in_plane(intersection, vertices, dir0))
        {
            err = EXIT_FAILURE;
            P3V_print(stderr, "refp0", refp0);
            P3V_print(stderr, "intersection0", intersection);
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "dir0 intersection mismatch");
        }
        if (!TESTEQUALP3V(refp0, intersection, testtol))
        {
            err = EXIT_FAILURE;
            P3V_print(stderr, "refp0", refp0);
            P3V_print(stderr, "intersection0", intersection);
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "dir0 intersection mismatch");
        }
        intersection = P3Vtriangle_line_intersection(vertices, indices[0], p0, dir1, work);
        if (intersection)
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "dir1 intersection was not NULL");
        }
        intersection = P3Vtriangle_line_intersection(vertices, indices[0], p0, dir2, work);
        if (intersection)
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "dir1 intersection was not NULL");
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
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
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Fail in Pvector_orthogonalintersection()");
        }
        if (EXIT_SUCCESS != (err = Pvector_plane_orthogonalintersection()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Fail in Pvector_plane_orthogonalintersection()");
        }
        if (EXIT_SUCCESS != (err = Pvector_plane_line_intersection()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Fail in Pvector_plane_line_intersection()");
        }
        if (EXIT_SUCCESS != (err = Pvector_triangle_line_intersection()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Fail in Pvector_plane_line_intersection()");
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}