#ifndef MESHTEST_H_
#define MESHTEST_H_
#ifdef __cplusplus
#include <cstdint>
extern "C" {
#else
#include <stdint.h>
#endif
/**
 * @brief calculate a rotationally symmetrical point of P3V_TriVert[0]. The rotation center is
 * the center of P3V_TriVert[1] and P3V_TriVert[2]. axis direction is normal to the plane of the triangle.
 * 
 * @param P3V_TriVert 
 * @param P3Vwork 
 * @return const float* 
 */
const float* RotationallySymmetricalPoint(const float* P3V_TriVert[], float* P3Vwork);

/**
 * @brief Calculate a homography matrix mapping from a triangle of a mesh element to a triangle half
 * of a texture parallelogram.
 * 
 * @param vertIndices [in] indices of triangle vertices
 * @param vertP3 [in] array of triangle vertex coordinates
 * @param vertTextureP2 [in] 
 * @param mwork [in] work area, float[3x3]
 * @return const float* 
 */
const float* Mesh_calchomography(
    const int32_t* vertIndices, const float* vertP3, const float* vertTextureP2, float* mwork);


#ifdef __cplusplus
}
#endif
#endif /* MESHTEST_H_ */
