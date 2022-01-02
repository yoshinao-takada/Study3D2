#ifndef PCAMERA0_H_
#define PCAMERA0_H_
#include "Pspc/Pmatrix.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    float size[2]; // image size counted in pixels
    float f35; // 35 mm equivalent focal length, unit [meter]
} Camera35mmConf_t, *pCamera35mmConf_t;
typedef const Camera35mmConf_t *pcCamera35mmConf_t;

/**
 * @brief get a camera intrinsic matrix in 3x4 size
 * 
 * @param camera 
 * @param intrin3x4 
 */
void Camera35mmConf_mat(pcCamera35mmConf_t camera, float* intrin3x4);

// typedef struct {
//     float size[2]; // image size counted in pixels
//     float FOVy;
//     float sizeN[2]; // frustum near Z-plane size
// } CameraGLFrustumConf_t, *pCameraGLFrustumConf_t;
// typedef const CameraGLFrustumConf_t *pcCameraGLFrustumConf_t;

/**
 * @brief get a viewline corresponding to a viewport coordinate
 * 
 * @param mat3x4 [in] Projection matrix
 * @param vpcoord [in] 2D viewport coordinate
 * @param cameraposition [in] camera position
 * @param p0 [out] P3V world coordinate of the viewline reference point
 * @param dir [out] P3V viewline direction vector
 */
void CameraViewline(const float* mat3x4, const float* vpcoord, pcP3MCameraPosition_t cameraposition,
    float* p0, float* dir);
#ifdef __cplusplus
}
#endif
#endif /* PCAMERA0_H_ */