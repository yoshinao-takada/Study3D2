#include "Render/mesh.h"
#include "Render/meshtest.h"
#include "Util/testutils.h"
#define VERT_SQUARE { \
    0.0f, 0.0f, 0.0f, 1.0f, /* (0,0,0) */ \
    1.0f, 0.0f, 0.0f, 1.0f, /* (1,0,0) */ \
    0.0f, 1.0f, 0.0f, 1.0f, /* (0,1,0) */ \
    1.0f, 1.0f, 0.0f, 1.0f /* (1,1,0) */ }

#define VERT_ANGLE_TRIANGLES { \
    0.0f, 0.0f, 0.0f, 1.0f, /* (0,0,0) */ \
    1.0f, 0.0f, 0.0f, 1.0f, /* (1,0,0) */ \
    0.5f, 0.5f, 0.707f, 1.0f, /* (0.5,0.5,sqrt(2)*0.5) */ \
    1.0f, 1.0f, 0.0f, 1.0f /* (1,1,0) */ }

#define VERT_INDICES { \
    0, 1, 3, \
    0, 3, 2 }

#define TEXTURE_COORDS { \
    0.0f, 0.0f, 1.0f, \
    1.0f, 0.0f, 1.0f, \
    0.0f, 1.0f, 1.0f, \
    1.0f, 1.0f, 1.0f }


static int RotationallySymmetricalPointUT()
{
    int err = EXIT_SUCCESS;
    do {

    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

static int Mesh_calchomographyUT()
{
    int err = EXIT_SUCCESS;
    do {

    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}


static int Mesh_crossUT()
{
    int err = EXIT_SUCCESS;
    do {

    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

static int Mesh_textureUT()
{
    int err = EXIT_SUCCESS;
    do {

    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}

int meshUT()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = RotationallySymmetricalPointUT()))
        {
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "Fail in RotationallySymmetricalPointUT()");
        }
        if (EXIT_SUCCESS != (err = Mesh_calchomographyUT()))
        {
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "Fail in Mesh_calchomographyUT()");
        }
        if (EXIT_SUCCESS != (err = Mesh_crossUT()))
        {
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "Fail in Mesh_crossUT()");
        }
        if (EXIT_SUCCESS != (err = Mesh_textureUT()))
        {
            LOGERRORBREAK(stderr, __FILE__, __LINE__, "Fail in Mesh_textureUT()");
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}