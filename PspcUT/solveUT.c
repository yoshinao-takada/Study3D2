#include "NLSL/NLSLmatrix.h"
#include "Util/testutils.h"
#include <stdlib.h>

static const float Msrc[] =
{
    0.0f, 1.2f, 2.5f, 3.3f,
    1.5f, 1.5f, 2.5f, 3.3f,
    1.2f, 2.3f, 2.4f, 2.5f,
    1.0f, 0.0f, 0.0f, 1.0f,
    2.5f, 2.5f, 0.0f, -3.3f
};

static const float Xref[] =
{
    2.5f, -2.5f,
    0.3f, 3.3f,
    1.5f, -2.5f,
    -0.2f, 0.2f
};

static const float tol = 1.0e-4f;

int solveUT()
{
    float ywork[10];
    float xwork[8];
    NLSLmatrix_t m = { 5, 4, {Msrc}};
    NLSLmatrix_t xref = { 4, 2, {Xref}};
    NLSLmatrix_t y = { 5, 2, { ywork }};
    NLSLmatrix_t x = { 4, 2, { xwork }};
    int err = EXIT_SUCCESS;
    do {
        NLSLmatrix_mult(&m, &xref, &y);
        NLSLmatrix_solve(&m, &y, &x);
        int equal = 1;
        for (int row = 0; row != x.rows; row++)
        {
            for (int column = 0; column != x.columns; column++)
            {
                int index = column + row * x.columns;
                equal = equal &&
                    TESTEQUALF(x.elements.c[index], xref.elements.c[index], tol);
                if (equal == 0)
                {
                    err = EXIT_FAILURE;
                    LOGERRORBREAK(stderr, __FILE__, __LINE__, "xref(row=%d, column=%d) = %f, x(row=%d, column=%d) = %f",
                        row, column, xref.elements.c[index], row, column, x.elements.c[index]);
                }
            }
            if (err) break;
        }
        if (err) break;
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}