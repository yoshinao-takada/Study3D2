#include "Pspc/homography.h"
#include "Pspc/Pvector.h"
#include "Util/testutils.h"
#include <stdlib.h>
#include "NLSL/NLSLutils.h"
#include "NLSL/NLSLmatrix.h"
#include <memory.h>

#define PJO 0.5f /* Projection offset */
/*
[NOTE]: homography matrix require its source points are projected to planes.
Therefore z is not allowed to be zero. Adding a constant offset to z means that
the projection camera moves appart from z-plane.
*/
static const float src0[] =
{
   1.0f, 0.0f, 0.0f + PJO, 1.0f,
   0.0f, 1.0f, 0.0f + PJO, 1.0f,
   0.0f, 0.0f, 10.0f + PJO, 1.0f
};

static const float dst0[] = 
{
   0.0f, 0.0f, 0.0f + PJO, 1.0f,
   1.0f, 0.0f, 0.0f + PJO, 1.0f,
   0.0f, 1.0f, 0.0f + PJO, 1.0f
};

static const float dst1[] =
{
   0.0f, 0.0f, 1.0f,
   1.0f, 0.0f, 1.0f,
   0.0f, 1.0f, 1.0f
};

static const float tol = 1.0e-4f;

static int AnotherPointUT()
{
    int err = EXIT_SUCCESS;
    do {
        P3Triangle_t triangle = { PxV_P3V, { src0, src0 + P3VSIZE, src0 + 2*P3VSIZE } };
        float another_[P3VSIZE], sumVert0AndAnother[P3VSIZE], sumVert1And2[P3VSIZE];
        const float* another = P3TriangleHomology_AnotherPoint(&triangle, another_);
        P3V_add(triangle.vert[0], another, sumVert0AndAnother);
        P3V_add(triangle.vert[1], triangle.vert[2], sumVert1And2);
        if (!TESTEQUALP3V(sumVert0AndAnother, sumVert1And2, tol))
        {
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "sumVert0AndAnother != sumVert1And2");
        }
        triangle.type = PxV_R3V;
        triangle.vert[0] = dst0; triangle.vert[1] = dst0 + P3VSIZE; triangle.vert[2] = dst0 + 2 * P3VSIZE;
        another = P3TriangleHomology_AnotherPoint(&triangle, another_);
        memcpy(sumVert0AndAnother, triangle.vert[0], sizeof(float) * R3VSIZE);
        NLSLutils_addfloats(R3VSIZE, sumVert0AndAnother, another);
        memcpy(sumVert1And2, triangle.vert[1], sizeof(float) * R3VSIZE);
        NLSLutils_addfloats(R3VSIZE, sumVert1And2, triangle.vert[2]);
        if (!TESTEQUALR3V(sumVert0AndAnother, sumVert1And2, tol))
        {
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "sumVert0AndAnother != sumVert1And2");
        }

        triangle.type = PxV_P2V;
        triangle.vert[0] = dst1; triangle.vert[1] = dst1 + P2VSIZE; triangle.vert[2] = dst1 + P2VSIZE;
        another = P3TriangleHomology_AnotherPoint(&triangle, another_);
        P2V_add(triangle.vert[0], another, sumVert0AndAnother);
        P2V_add(triangle.vert[1], triangle.vert[2], sumVert1And2);
        if (!TESTEQUALP2V(sumVert0AndAnother, sumVert1And2, tol))
        {
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "sumVert0AndAnother != sumVert1And2");
        }

        triangle.type = PxV_R2V;
        another = P3TriangleHomology_AnotherPoint(&triangle, another_);
        memcpy(sumVert0AndAnother, triangle.vert[0], sizeof(float) * R2VSIZE);
        NLSLutils_addfloats(R2VSIZE, sumVert0AndAnother, another);
        memcpy(sumVert1And2, triangle.vert[1], sizeof(float) * R2VSIZE);
        NLSLutils_addfloats(R2VSIZE, sumVert1And2, triangle.vert[2]);
        if (!TESTEQUALR2V(sumVert0AndAnother, sumVert1And2, tol))
        {
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "sumVert0AndAnother != sumVert1And2");
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

static int HomographyMatrixUT()
{
    int err = EXIT_SUCCESS;
    float h_[R3VSIZE * R3VSIZE];
    float vdst_[R3VSIZE];
    do  {
        P3TriangleHomology_t homology = {
            { PxV_P3V, { src0, src0 + P3VSIZE, src0 + 2*P3VSIZE } },
            { PxV_P3V, { dst0, dst0 + P3VSIZE, dst0 + 2*P3VSIZE } }
        };
        NLSLmatrix_t h = { 3, 3, { P3TriangleHomology_homographymatrix(&homology, h_)} };
        NLSLmatrix_t dst = { 3, 1, { vdst_ } };
        NLSLmatrix_t src = { 3, 1, { src0 } };
        NLSLmatrix_mult(&h, &src, &dst);
        if (!TESTEQUALP2V(dst.elements.c, dst0, tol))
        {
            err = EXIT_FAILURE;
            fprintf(stderr, "h * src0 : ");
            NLSLmatrix_print(stderr, &dst);
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "src0 conversion result mismatch");
        }
        src.elements.c += P3VSIZE;
        NLSLmatrix_mult(&h, &src, &dst);
        if (!TESTEQUALP2V(dst.elements.c, dst0 + P3VSIZE, tol))
        {
            err = EXIT_FAILURE;
            fprintf(stderr, "h * src0 : ");
            NLSLmatrix_print(stderr, &dst);
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "src0 conversion result mismatch");
        }
        src.elements.c += P3VSIZE;
        NLSLmatrix_mult(&h, &src, &dst);
        if (!TESTEQUALP2V(dst.elements.c, dst0 + 2 * P3VSIZE, tol))
        {
            err = EXIT_FAILURE;
            fprintf(stderr, "h * src0 : ");
            NLSLmatrix_print(stderr, &dst);
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "src0 conversion result mismatch");
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

int homographyUT()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = AnotherPointUT()))
        {
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "AnotherPointUT() = %d", err);
        }
        if (EXIT_SUCCESS != (err = HomographyMatrixUT()))
        {
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "HomographyMatrixUT() = %d", err);
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

