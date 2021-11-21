#include "Pspc/Pmatrix.h"
#include "Pspc/Pvector.h"
#include "Util/testutils.h"

// basic operations
static int Pmatrix_basicops()
{
    int err = EXIT_SUCCESS;
    do {

    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

// basic transform matrices
static int Pmatrix_basictrans()
{
    int err = EXIT_SUCCESS;
    do {

    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

// composite transform matrices
static int Pmatrix_compositetrans()
{
    int err = EXIT_SUCCESS;
    do {

    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

// camera operations
static int Pmatrix_cameraops()
{
    int err = EXIT_SUCCESS;
    do {

    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

int PmatrixUT()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = Pmatrix_basicops()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "fail in Pmatrix_basicops()");
        }
        if (EXIT_SUCCESS != (err = Pmatrix_basictrans()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "fail in Pmatrix_basictrans()");
        }
        if (EXIT_SUCCESS != (err = Pmatrix_compositetrans()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "fail in Pmatrix_compositetrans()");
        }
        if (EXIT_SUCCESS != (err = Pmatrix_cameraops()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "fail in Pmatrix_cameraops()");
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}