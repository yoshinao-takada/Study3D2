#include "Pspc/Pcamera0.h"
#include "NLSL/NLSLutils.h"
#include "Pspc/Pvector.h"
#include "Pspc/Pmatrix.h"
#include "NLSL/NLSLmatrix.h"
#include <memory.h>
#include <math.h>

void Camera35mmConf_mat(pcCamera35mmConf_t camera, float* intrin3x4)
{
    const float W35 = 36e-3f;
    const float H35 = 24e-3f;
    const float D35 = sqrtf(W35 * W35 + H35 * H35);
    const float D = sqrtf(camera->size[0] * camera->size[0] + camera->size[1] * camera->size[1]);
    const float X_CTR = camera->size[0] * D35 / (2.0f * D);
    const float Y_CTR = camera->size[1] * D35 / (2.0f * D);
    const float X_CBL = -X_CTR;
    const float Y_CBL = -Y_CTR;
    const float Z_CTR = -camera->f35;
    float workMat[2 * 4 * 4];
    float workVec[4 + 4];
    NLSLmatrix_t LHS = { 4, 4, {workMat}};
    NLSLmatrix_t LHSinv = { 4, 4, {workMat + 16}};
    NLSLmatrix_t RHS = { 4, 1, {workVec}};
    NLSLmatrix_t sol = { 4, 1, {workVec + 4}};
    NLSL_FILLFLOATS(workMat, 0.0f, NLSL_ARRAYSIZE(workMat));
    NLSL_FILLFLOATS(workVec, 0.0f, NLSL_ARRAYSIZE(workVec));
    NLSL_FILLFLOATS(intrin3x4, 0.0f, 3 * 4);
    
    float* LHSe = LHS.elements.v;
    LHSe[0] = X_CTR / Z_CTR;
    LHSe[2] = 1.0f;
    LHSe += 4;
    LHSe[1] = Y_CTR / Z_CTR;
    LHSe[3] = 1.0f;
    LHSe += 4;
    LHSe[0] = X_CBL / Z_CTR; // Z_CBL is equal to Z_CTR and was omitted to declare.
    LHSe[2] = 1.0f;
    LHSe += 4;
    LHSe[1] = Y_CBL / Z_CTR;
    LHSe[3] = 1.0f;
    RHS.elements.v[0] = camera->size[0];
    RHS.elements.v[3] = camera->size[1];
    NLSLmatrix_inv(&LHS, &LHSinv);
    NLSLmatrix_mult(&LHSinv, &RHS, &sol);

    intrin3x4[0] = sol.elements.c[0];
    intrin3x4[5] = sol.elements.c[1];
    intrin3x4[2] = sol.elements.c[2];
    intrin3x4[6] = sol.elements.c[3];
    intrin3x4[10] = 1.0f;
}

void CameraViewline(const float* mat3x4, const float* vpcoord, pcP3MCameraPosition_t cameraposition,
    float* p0, float* dir
) {
    // get normal vectors of planes
    float vn0[] = {
        vpcoord[0] * mat3x4[8] - mat3x4[0], vpcoord[0] * mat3x4[9] - mat3x4[1], vpcoord[0] * mat3x4[10] - mat3x4[2],
        1.0f
    };
    float vn1[] = {
        vpcoord[1] * mat3x4[8] - mat3x4[4], vpcoord[1] * mat3x4[9] - mat3x4[5], vpcoord[1] * mat3x4[10] - mat3x4[6],
        1.0f
    };

    float lookatDir[P3VSIZE], vdir[P3VSIZE];
    P3V_cross(vn0, vn1, vdir); // cross product of the normals is aligned to the plane intersection line.
    P3V_sub(cameraposition->lookat, cameraposition->position, lookatDir); // get the camera look-at direction vector
    float signFlag = P3V_dot(vdir, lookatDir); // dot product of the viewline and camera look-at direction vector
    if (signFlag < 0.0f)
    { // if negative, reverse direction
        vdir[0] = -vdir[0]; vdir[1] = -vdir[1]; vdir[2] = -vdir[2];
    }
    P3V_normalize(vdir, dir);
    memcpy(p0, cameraposition->position, P3VSIZE * sizeof(float));
}