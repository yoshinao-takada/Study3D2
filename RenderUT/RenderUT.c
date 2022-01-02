#include "Util/testutils.h"
#include <stdio.h>
#include <stdlib.h>

int textureUT();
int meshUT();
int projectionUT();

int main(int argc, const char* *argv)
{
    int err = EXIT_SUCCESS;
    do {
        // if (EXIT_SUCCESS != (err = textureUT()))
        // {
        //     LOGERRORBREAK(stderr, __FILE__, __LINE__, "textureUT() = %d", err);
        // }
        // if (EXIT_SUCCESS != (err = meshUT()))
        // {
        //     LOGERRORBREAK(stderr, __FILE__, __LINE__, "meshUT() = %d", err);
        // }
        if (EXIT_SUCCESS != (err = projectionUT()))
        {
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "projectionUT() = %d", err);
        }
    } while (0);
    return err;
}