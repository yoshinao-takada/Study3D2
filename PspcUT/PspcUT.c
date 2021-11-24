#include "Util/testutils.h"
int PvectorUT();
int PmatrixUT();
int solveUT();

int main()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = PvectorUT()))
        {
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "PvectorUT() = %d\n", err);
        }
        if (EXIT_SUCCESS != (err = PmatrixUT()))
        {
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "PmatrixUT() = %d\n", err);
        }
        if (EXIT_SUCCESS != (err = solveUT()))
        {
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "solveUT() = %d\n", err);
        }
    } while (0);
    return err;
}