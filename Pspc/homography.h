#ifndef HOMOGRAPHY_H_
#define HOMOGRAPHY_H_
#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
    PxV_P3V, // (x, y, z, w) homogeneous coordinate
    PxV_R3V, // (x, y, z) real 3D coordinate
    PxV_P2V, // (x, y, w) plane homogenous coordinate
    PxV_R2V // (x, y) real 2D coordinate
} PxV_Type_t;

typedef struct {
    PxV_Type_t type;
    const float* vert[3];
} P3Triangle_t, *pP3Triangle_t;
typedef const P3Triangle_t *pcP3Triangle_t;

typedef struct {
    P3Triangle_t src; // mapping source
    P3Triangle_t dst; // mapping destination
} P3TriangleHomology_t, *pP3TriangleHomology_t;
typedef const P3TriangleHomology_t *pcP3TriangleHomology_t;

/**
 * @brief calculate a homography matrix based on a homology of two triangles
 * 
 * @param homology [in] triangle pair
 * @param work [in] 3x3 matrix work area
 * @return const float* 
 */
const float* P3TriangleHomology_homographymatrix(pcP3TriangleHomology_t homology, float* work);

/**
 * @brief calculate the 4th corner of a parallelogram opposing to triangle->vert[0]
 * 
 * @param triangle [in]
 * @param work [in] work area size depends on triangle->type
 * @return const float* casted from work where the 4th point coordinate is put
 */
const float* P3TriangleHomology_AnotherPoint(pcP3Triangle_t triangle, float* work);
#ifdef __cplusplus
}
#endif
#endif /* HOMOGRAPHY_H_ */