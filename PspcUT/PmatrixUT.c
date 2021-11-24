#include "Pspc/Pmatrix.h"
#include "Pspc/Pvector.h"
#include "Util/testutils.h"
#include "PspcUT/PmatrixUTData.h"
#define __USE_GNU
#include <math.h>
#include <memory.h>


static const float tol = 1.0e-4f;
static const float tollarge = 1.0e-3f;
static const float tolsmall = 1.0e-4f;

// basic operations
static int Pmatrix_basicops()
{
    static const float imat[] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    static const float mat0[] = {
        2.2f, 3.5f, -1.5f, 0.2f,
        1.5f, 3.3f, 0.7f, -1.5f,
        8.5f, 1.5f, -1.1f, 0.0f,
        0.0f, 8.5f, 1.5f, -2.5f
    };
    int err = EXIT_SUCCESS;
    float work0[P3MSIZE];
    float work1[P3MSIZE];
    do {
        const float *mat0inv = P3M_inv(mat0, work0);
        const float *testimat = P3M_mult(mat0, mat0inv, work1);
        int equal = 1;
        const float* refimat = imat;
        for (int row = 0; row < P3MROWS; row++)
        {
            for (int column = 0; column < P3MCOLUMNS; column++)
            {
                // apply more strict tolerance to diagonal elements than to non-diagonal elements.
                const float tol = (row == column) ? tolsmall : tollarge;
                equal = equal && TESTEQUALF(*refimat, *testimat, tol);
                refimat++;
                testimat++;
            }
        }
        if (!equal)
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Fail in P3M_inv() or P3M_mult()");
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

static int Pmatrix_basicops2()
{
    static const float mat0[] = {
        0.2f, 1.5f, -1.2f,
        0.7f, -0.7f, -2.0f,
        -2.0f, -1.0f, 0.0f
    };
    static const float imat[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    int err = EXIT_SUCCESS;
    float work0[P2MSIZE];
    float work1[P2MSIZE];
    do {
        const float* mat0inv = P2M_inv(mat0, work0);
        const float* testimat = P2M_mult(mat0, mat0inv, work1);
        const float* refimat = imat;
        int equal = 1;
        for (int row = 0; row != P2MROWS; row++)
        {
            for (int column = 0; column != P2MCOLUMNS; column++)
            {
                const float tol = (row == column) ? tolsmall : tollarge;
                equal = equal && TESTEQUALF(*refimat, *testimat, tol);
                refimat++;
                testimat++;
            }
        }
        if (!equal)
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Fail in P2M_inv() or P2M_mult()");
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}
// basic transform matrices
static int Pmatrix_basictrans()
{
    static const float from[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    static const float to[] = { 3.0f, 2.0f, 2.0f, 2.0f };
    static const float deltaref[] = { 1.0f, 2.0f, 2.0f, 2.0f }; // = to - from
    static const float from1[] = { 1.5, 2.5, 33.0f, 1.5f };
    float work_mtransport[P3MSIZE];
    float work_vtotest[P3MROWS];
    float work_vdeltatest[P3MROWS];
    int err = EXIT_SUCCESS;
    do {
        const float *mtransport = P3M_transport(from, to, work_mtransport);
        const float* vtotest = P3MP3V_mult(mtransport, from, work_vtotest);
        if (!TESTEQUALP3V(to, vtotest, tol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Fail in P3M_transport()");
        }
        vtotest = P3MP3V_mult(mtransport, from1, work_vtotest);
        const float* vdeltatest = P3V_sub(vtotest, from1, work_vdeltatest);
        if (!TESTEQUALP3V(deltaref, vdeltatest, tol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Fail in P3M_transport()");
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

// rotation about X axis
static int Pmatrix_rotx()
{
    static const float from[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    static const float toref[] = { 1.0f, 0.0f, 1.414213562373095048801688724209698079f, 1.0f };
    float mwork[P3MSIZE];
    float vwork[P3MROWS];
    int err = EXIT_SUCCESS;
    do {
        const float* mrot = P3M_rotateX(0.785398163397448309615660845819875721f, mwork);
        const float* vtest = P3MP3V_mult(mrot, from, vwork);
        if (!TESTEQUALP3V(toref, vtest, tol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Fail in P3M_rotateX()");
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

// rotation about Y axis
static int Pmatrix_roty()
{
    static const float from[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    static const float toref[] = { 1.414213562373095048801688724209698079f, 1.0f, 0.0f, 1.0f };
    float mwork[P3MSIZE];
    float vwork[P3MROWS];
    int err = EXIT_SUCCESS;
    do {
        const float* mrot = P3M_rotateY(0.785398163397448309615660845819875721f, mwork);
        const float* vtest = P3MP3V_mult(mrot, from, vwork);
        if (!TESTEQUALP3V(toref, vtest, tol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Fail in P3M_rotateY()");
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

// rotation about Z axis
static int Pmatrix_rotz()
{
    static const float from[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    static const float toref[] = { 0.0f, 1.414213562373095048801688724209698079f, 1.0f, 1.0f };
    float mwork[P3MSIZE];
    float vwork[P3MROWS];
    int err = EXIT_SUCCESS;
    do {
        const float* mrot = P3M_rotateZ(0.785398163397448309615660845819875721f, mwork);
        const float* vtest = P3MP3V_mult(mrot, from, vwork);
        if (!TESTEQUALP3V(toref, vtest, tol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Fail in P3M_rotateZ()");
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

// composite transform matrices
static int Pmatrix_align()
{
    static const float from0[] = { 1.0f, 2.0f, -1.0f, 0.5f };
    static const float from1[] = { -1.0f, -2.0f, 1.0f, 0.7f };
    static const float to[] = { 1.0f, 0.7f, 1.0f, 1.0f };
    float mwork[P3MSIZE];
    float vwork[P3MROWS];
    int err = EXIT_SUCCESS;
    do {
        const float* align0 = P3M_align(from0, to, mwork);
        const float* aligned0 = P3MP3V_mult(align0, from0, vwork);
        const float from0Length = P3V_dot(from0, from0);
        const float aligned0Length = P3V_dot(aligned0, aligned0);
        const float toazimuth = R3V_azimuth(to);
        const float topolar = R3V_polar(to);
        const float aligned0azimuth = R3V_azimuth(aligned0);
        const float aligned0polar = R3V_polar(aligned0);
        if (!TESTEQUALF(from0Length, aligned0Length, tol) || !TESTEQUALF(toazimuth, aligned0azimuth, tol) || !TESTEQUALF(topolar, aligned0polar, tol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Fail in P3M_align()");
        }
        const float* align1 = P3M_align(from1, to, mwork);
        const float* aligned1 = P3MP3V_mult(align1, from1, vwork);
        const float from1Length = P3V_dot(from1, from1);
        const float aligned1Length = P3V_dot(aligned1, aligned1);
        const float aligned1azimuth = R3V_azimuth(aligned1);
        const float aligned1polar = R3V_polar(aligned1);
        if (!TESTEQUALF(from1Length, aligned1Length, tol) || !TESTEQUALF(toazimuth, aligned1azimuth, tol) || !TESTEQUALF(topolar, aligned1polar, tol))
        {
            err = EXIT_FAILURE;
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Fail in P3M_align()");
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

typedef struct {
    float axP0[4];
    float axDir[4];
    float p1[4];
} RotationTestData_t, *pRotationTestData_t;
typedef const RotationTestData_t *pcRotationTestData_t;

/**
 * @brief rotate p1 about axis (axP0, axDir), by M_PI_4 twice, totally M_PI_2.
 * After rotation, check orthgonality of the axis-p1 and axis-p1'. p1' is a destination of
 * p1 after rotation.
 * 
 * @return int EXIT_SUCCESS or EXIT_FAILURE
 */
static int Pmatrix_rotateaboutaxis(pcRotationTestData_t data)
{
    int err = EXIT_SUCCESS;
    float mwork[P3MSIZE];
    float vwork[2 * P3MROWS];
    float vcrosspointref[2 * P3MROWS];
    float vcrosspointtest[2 * P3MROWS];
    float vcrossprodnormals[P3MROWS];
    const float m_pi_4 = atan2f(1.0f, 1.0f);
    do {
        const float* m = P3M_rotateaboutaxis(m_pi_4, data->axP0, data->axDir, mwork);
        const float* v = P3MP3V_mult(m, P3MP3V_mult(m, data->p1, vwork), vwork + P3MROWS); // rotate 90 degs
        // check identity of orthgonal crossing point
        const float* crossref = P3V_orthogonalintersection(data->axP0, data->axDir, data->p1, vcrosspointref);
        const float* crosstest = P3V_orthogonalintersection(data->axP0, data->axDir, v, vcrosspointtest);
        if (!TESTEQUALP3V(crossref, crosstest, tol))
        {
            err = EXIT_FAILURE;
            P3V_print(stderr, "crossref", crossref);
            P3V_print(stderr, "crosstest", crosstest);
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "axis intersection not equal");
        }

        // check orthogonarity of ref axis, and points before rotation and after rotation
        const float rotangletest = P3V_dot(crossref + P3MROWS, crosstest + P3MROWS);
        if (fabsf(rotangletest) > tol)
        {
            err = EXIT_FAILURE;
            P3V_print(stderr, "crossref-normal", crossref + P3MROWS);
            P3V_print(stderr, "crosstest-normal", crosstest + P3MROWS);
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "normals are not orthogonal");
        }

        // check if rotated vector cross product is aligned to the rotation axis
        const float* crossprod = P3V_cross(crossref + P3MROWS, crosstest + P3MROWS, vcrossprodnormals);
        const float crossproddotrefaxis = P3V_dot(crossprod, data->axDir);
        const float length = sqrtf(P3V_dot(crossprod, crossprod) * P3V_dot(data->axDir, data->axDir));
        if (!TESTEQUALF(length, crossproddotrefaxis, tol))
        {
            err = EXIT_FAILURE;
            fprintf(stderr, "length = %f, crossprod * data->axDir = %f\n", length, crossproddotrefaxis);
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "rotation direction invalid");
        }
    } while (0);
    return err;
}

static int Pmatrix_rotateaboutaxes()
{
    static const RotationTestData_t data[] = AXES_AND_POINTS;
    int dataCount = AXES_AND_POINTS_COUNT;
    int err = EXIT_SUCCESS;
    do {
        for (int i = 0; i < dataCount; i++)
        {
            err = Pmatrix_rotateaboutaxis(&data[i]);
            if (err != EXIT_SUCCESS)
            {
                LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Fail in Pmatrix_rotateaboutaxis()");
            }
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

static int Pmatrix_compositetrans()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = Pmatrix_align()))
        {
            break;
        }
        if (EXIT_SUCCESS != (err = Pmatrix_rotateaboutaxes()))
        {
            break;
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

// camera operations

int PmatrixUT()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = Pmatrix_basicops()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "fail in Pmatrix_basicops()");
        }
        if (EXIT_SUCCESS != (err = Pmatrix_basicops2()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "fail in Pmatrix_basicops2()");
        }
        if (EXIT_SUCCESS != (err = Pmatrix_basictrans()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "fail in Pmatrix_basictrans()");
        }
        if (EXIT_SUCCESS != (err = Pmatrix_rotx()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "fail in Pmatrix_rotx()");
        }
        if (EXIT_SUCCESS != (err = Pmatrix_roty()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "fail in Pmatrix_roty()");
        }
        if (EXIT_SUCCESS != (err = Pmatrix_rotz()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "fail in Pmatrix_rotz()");
        }
        if (EXIT_SUCCESS != (err = Pmatrix_compositetrans()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "fail in Pmatrix_compositetrans()");
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}