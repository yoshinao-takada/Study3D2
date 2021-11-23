#ifndef PVECTOR_H_
#define PVECTOR_H_
/**
 * @brief Basic functions of vectors in projection space
 * 
 */
#ifdef __cplusplus
extern "C" {
#endif

#define P3VSIZE     4
#define P2VSIZE     3
#define R3VSIZE     3
#define R2VSIZE     2

/**
 * @brief addion of P3 vector
 * 
 * @param v0 
 * @param v1 
 * @param vwork 
 * @return const float* 
 */
const float* P3V_add(const float* v0, const float* v1, float* vwork);

/**
 * @brief subraction of P3 vector
 * 
 * @param v0 
 * @param v1 
 * @param vwork 
 * @return const float* 
 */
const float* P3V_sub(const float* v0, const float* v1, float* vwork);

/**
 * @brief dot product of two P3 vectors
 * 
 * @param v0 
 * @param v1 
 * @return float 
 */
float P3V_dot(const float* v0, const float* v1);

/**
 * @brief cross product of two P3 vectors
 * 
 * @param v0 
 * @param v1 
 * @param vwork [in] float[3] work area is converted to const float* to return.
 * @return const float* 
 */
const float* P3V_cross(const float* v0, const float* v1, float* vwork);

/**
 * @brief azimuth angle of v, rotation axis: Y-axis, angle-0 axis: Z-axis positive
 *
 * @param v [in] P3 and R3 vector are acceptable.
 * @return azimuth angle in radian
 */
float R3V_azimuth(const float* v);

/**
 * @brief polar angle of v, angle-0 axis: Y-axis positive, reference plane: XZ
 * 
 * @param v [in] P3 and R3 vector are acceptable.
 * @return elevation angle in radian
 */
float R3V_polar(const float* v);

/**
 * @brief convert P3(projection 3D space) vector to R3(real 3D space) vector
 * 
 * @param v [in] P3 vector
 * @param vwork [in] work area convert to const float* to return
 * @return const float* R3 vector
 */
const float* P3V_toR3V(const float* v, float* vwork);

/**
 * @brief convert P2(projection 2D space) vector to R2(real 2D space) vector
 * 
 * @param v [in] P2 vector
 * @param vwork [in] work area convert to const float* to return
 * @return const float* R2 vector
 */
const float* P2V_toR2V(const float* v, float* vwork);

/**
 * @brief normalize a P3 vector
 * 
 * @param v 
 * @param vwork 
 * @return const float* 
 */
const float* P3V_normalize(const float* v, float* vwork);

/**
 * @brief draw a line orthogonal and crossing to a reference line
 * 
 * @param p0 [in] a reference point of the reference line
 * @param dir [in] reference line direction vector
 * @param p1 [in] a point to draw the orthogonal line
 * @param vwork [in] work area convert to const float* to return, capacity = 8 * sizeof(float)
 * @return const float* P3 intersection point and direction vector of the orthgonal line,
 *  return pointer is the intersection point, return pointer + 4 is the direction vector.
 */
const float* P3V_orthogonalintersection(const float* p0, const float* dir, const float* p1, float* vwork);

#define P3V_print(_out_, _caption_, _v_) fprintf(_out_, "%s, P3V:{ %f, %f, %f, %f }\n", (_caption_), \
    (_v_)[0], (_v_)[1], (_v_)[2], (_v_)[3] );
#ifdef __cplusplus
}
#endif
#endif /* PVECTOR_H_ */