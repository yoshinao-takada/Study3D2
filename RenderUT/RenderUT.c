#include "Util/testutils.h"
#include <stdio.h>
#include <stdlib.h>

int meshUT();
int textureUT();

int main(int argc, const char* *argv)
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = meshUT()))
        {
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "meshUT() = %d", err);
        }
        if (EXIT_SUCCESS != (err = textureUT()))
        {
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "textureUT() = %d", err);
        }
    } while (0);
    return err;
}