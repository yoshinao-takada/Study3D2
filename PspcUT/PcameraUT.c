#include "Pspc/Pcamera.h"
#include "Pspc/Pmatrix.h"
#include "Pspc/Pvector.h"
#include "NLSL/NLSLmatrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>

#pragma region camera_operations
static const P3MCamera_t camera0 = {
    { 0.0f, 0.0f },
    { 640, 480 },
    1.57079632679489661923f,
    FOVaxis_x,
    0.0f
};

static const P3MCamera_t camera1 = {
    { 0.0f, 1080.0f },
    { 1920.0f, 0.0f },
    (120.0f * 90.0f) * 1.57079632679489661923f,
    FOVaxis_x,
    0.0f
};

static void copy3x4to3x3(float* dst, const float* src)
{
    size_t copysize = P2MCOLUMNS * sizeof(float);
    for (int row = 0; row < P2MROWS; row++)
    {
        memcpy(dst, src, copysize);
        dst += P2MCOLUMNS;
        src += P3MCOLUMNS;
    }
}

static int Pmatrix_cameraintrinsics()
{
    int err = EXIT_SUCCESS;
    float mwork3x4[P2MROWS * P3MCOLUMNS];
    float mwork0[P2MSIZE];
    float mwork1[P2MSIZE];
    const float P3xE = tanf(camera0.fov / 2.0f);
    const float P3points[][P3VSIZE] = {
        {  }, // bottom-left
        { }, // bottom-right
        { }, // top-left
        { } // top-right
    };
    const float R2points[][2] =
    {
        { 0.0f, 0.0f }, // bottom-left
        { 640.0f, 0.0f }, // bottom-right
        { 0.0f, 480.0f }, // top-left
        { 640.0f, 480.0f } // top-right
    };
    do {
        const float* cameraintrinsics = P3M_cameraintrinsics(&camera0, mwork3x4);

    } while (0);
    return err;
}
static int Pmatrix_cameraops()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = Pmatrix_cameraintrinsics()))
        {
            LOGERRORBREAK(stderr, __FUNCTION__, __LINE__, "Fail in Pmatrix_cameraintrinsics()");
        }
    } while (0);
    LOGERROR(stderr, __FUNCTION__, __LINE__, "err = %d", err);
    return err;
}
#pragma endregion camera_operations

int PcameraUT()
{
    int err = EXIT_SUCCESS;
    do {

    } while (0);
    return err;
}