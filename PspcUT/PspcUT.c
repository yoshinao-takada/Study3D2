#include "Util/testutils.h"
int PvectorUT();
int PmatrixUT();
int solveUT();
int PcameraUT();
int homographyUT();
int Pcamera0UT();
int gnsolverUT();

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
        if (EXIT_SUCCESS != (err =PcameraUT()))
        {
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "PcameraUT() = %d\n", err);
        }
        if (EXIT_SUCCESS != (err = homographyUT()))
        {
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "homographyUT() = %d\n", err);
        }
        if (EXIT_SUCCESS != (err =Pcamera0UT()))
        {
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "Pcamera0UT() = %d\n", err);
        }
        if (EXIT_SUCCESS != (err =gnsolverUT()))
        {
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "gnsolverUT() = %d\n", err);
        }
    } while (0);
    return err;
}