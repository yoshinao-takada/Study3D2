#include "Util/testutils.h"
int PvectorUT();
int PmatrixUT();

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
    } while (0);
    return err;
}