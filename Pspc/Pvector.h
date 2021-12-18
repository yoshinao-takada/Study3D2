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
const float* P2V_add(const float* v0, const float* v1, float* vwork);

/**
 * @brief subraction of P3 vector
 * 
 * @param v0 
 * @param v1 
 * @param vwork 
 * @return const float* 
 */
const float* P3V_sub(const float* v0, const float* v1, float* vwork);
const float* P2V_sub(const float* v0, const float* v1, float* vwork);

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

/**
 * @brief alias of P3V_orthogonalintersection()
 * 
 * @param p0 
 * @param dir 
 * @param p1 
 * @param vwork 
 * @return const float* 
 */
extern const float* (*P3Vline_orthogonalintersection)(const float* p0, const float* dir, const float* p1, float* vwork);

/**
 * @brief draw a line orthogonal and crossing to a reference plane
 * 
 * @param p0 [in] a reference point of the reference plane
 * @param normaldir [in] reference plane normal vector
 * @param p1 [in] a point to draw the orthogonal line
 * @param vwork [in] work area convert to const float* to return
 * @return const float* P3 intersection point
 */
const float* P3Vplane_orthogonalintersection(const float* p0, const float* normaldir, const float* p1, float* vwork);

/**
 * @brief get an intersection coordinate of a plane and a line
 * 
 * @param p0plane [in] reference P3 point of a plane
 * @param normaldir [in] reference P3 normal vector of a plane
 * @param p0line [in] reference P3 point of a line
 * @param dir [in] reference P3 direction vector of a line
 * @param vwork [in] work area convert to a const float* to return, capacity = 4 * sizeof(float)
 * @return const float* P3 coordinate of the intersection
 */
const float* P3Vplane_line_intersection
(const float* p0plane, const float* normaldir, const float* p0line, const float* dir, float* vwork);

/**
 * @brief get an intersection coordinate of a triangle and a line
 * 
 * @param vertices [in] vertex coordinate table
 * @param indices [in] CCW indices of the triangle. indices[0..2] point vertices[indices[0..2] * P3VSIZE];
 * i.e. const float* ptr = vertices + indices[0] * P3VSIZE;// ptr[0]: x, ptr[1]: y, ptr[2]: z, ptr[3]: w of the triangle 1st vertex.
 * @param p0line [in] a line reference point
 * @param dir [in] a line direction vector
 * @param vwork [in] work area convert to a const float* to return
 * @return const float* NULL: no intersectoin was found, (const float*)vwork: coordinate of the intersection
 */
const float* P3Vtriangle_line_intersection
(const float* vertices, const int* indices, const float* p0line, const float* dir, float* vwork);

#pragma region diagnostic_util
#define P3V_print(_out_, _caption_, _v_) fprintf(_out_, "%s, P3V:{ %f, %f, %f, %f }\n", (_caption_), \
    (_v_)[0], (_v_)[1], (_v_)[2], (_v_)[3] );
#define P2V_print(_out_, _caption_, _v_) fprintf(_out_, "%s, P2V:{ %f, %f, %f }\n", (_caption_), \
    (_v_)[0], (_v_)[1], (_v_)[2] );
#pragma endregion diagnostic_util
#ifdef __cplusplus
}
#endif
#endif /* PVECTOR_H_ */