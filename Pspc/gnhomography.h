#ifndef GNHOMOGRAPHY_H_
#define GNHOMOGRAPHY_H_
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculate homography matrix using Gauss-Newton solver
 * 
 * @param pk [in] 2D triangle vertices in projected plane
 * @param pj [in] [X, Y, Z] part of 3D triangle vertices in world homogenous or real space
 * @param work [in] float[9] buffer for resulted homography matrix
 * @return const float* is casted from work.
 */
const float* gnhomography(const float* pk, const float* pj, float* work);

#ifdef __cplusplus
}
#endif
#endif /* GNHOMOGRAPHY_H_ */