#ifndef PMATRIX_H_
#define PMATRIX_H_
#ifdef __cplusplus
extern "C" {
#endif
#define P3MROWS     4
#define P3MCOLUMNS  4
#define P3MSIZE     (P3MROWS * P3MCOLUMNS)
#define P2MROWS     3
#define P2MCOLUMNS  3
#define P2MSIZE     (P2MROWS * P2MCOLUMNS)
/**
 * @brief multiply two matrices
 * 
 * @param matA [in]
 * @param matB [in]
 * @param mwork [in] work area of 4x4 matrix is converted to const float* to return
 * @return const float* 4x4 matrix of product of matA and matB
 */
const float* P3M_mult(const float* matA, const float* matB, float* mwork);

/**
 * @brief multiply a matrix and a column vector
 * 
 * @param matA [in]
 * @param vecB [in]
 * @param vwork [in] work area of 4x1 column vector is converted to const float* to return.
 * @return const float* 4x1 column vector of product of matA and vecB
 */
const float* P3MP3V_mult(const float* matA, const float* vecB, float* vwork);

/**
 * @brief multiply a matrix and a column vector
 * 
 * @param matA [in] 3x4 matrix
 * @param vecB [in] 4x1 column vector
 * @param vwork [in] 3x1 column vector work area
 * @return const float* 3x1 column vector of product of matA and vecB
 */
const float* P23MP3V_mult(const float* matA, const float* vecB, float* vwork);

/**
 * @brief Create a transport matrix from fromP3 to toP3
 * 
 * @param fromP3 [in] start point of transport in homogeneous coordinate
 * @param toP3 [in] end point of transport in homogeneous coordinate
 * @param mwork [in] work area of 4x4 matrix is converted to const float* to return
 * @return const float* 4x4 homogeneous transport matrix
 */
const float* P3M_transport(const float* fromP3, const float* toP3, float* mwork);

typedef enum {
    AxisX, // positive X-axis
    AxisY, // positive Y-axis
    AxisZ, // positive Z-axis
    AxisXNeg, // negative X-axis
    AxisYNeg, // negative Y-axis
    AxisZNeg, // negative Z-axis
    AxisUndefined
} Axis_t;

/**
 * @brief Create a rotation matrix about x-axis
 * 
 * @param angle [in]
 * @param mwork [in] work area of 4x4 matrix is converted to const float* to return
 * @return const float* 4x4 homogeneous rotation matrix
 */
const float* P3M_rotateX(float angle, float* mwork);

/**
 * @brief Create a rotation matrix about y-axis
 * 
 * @param angle [in]
 * @param mwork [in] work area of 4x4 matrix is converted to const float* to return
 * @return const float* 4x4 homogeneous rotation matrix
 */
const float* P3M_rotateY(float angle, float* mwork);

/**
 * @brief Create a rotation matrix about z-axis
 * 
 * @param angle [in]
 * @param mwork [in] work area of 4x4 matrix is converted to const float* to return
 * @return const float* 4x4 homogeneous rotation matrix
 */
const float* P3M_rotateZ(float angle, float* mwork);

/**
 * @brief Create a direction alignment matrix from fromP3 to toP3
 * 
 * @param fromP3 [in] start vector to rotate in homogeneous coordinate
 * @param toP3 [in] end vector to rotate in homogeneous coordinate
 * @param mwork [in] work area of 4x4 matrix is converted to const float* to return
 * @return const float* 4x4 homogeneous rotation matrix
 */
const float* P3M_align(const float* fromP3, const float* toP3, float* mwork);

/**
 * @brief Invert m
 * 
 * @param m [in] a non-singular P3M matrix
 * @param mwork [in] work area of 4x4 matrix is converted to const float* to return
 * @return const float* invert of m, converted from mwork.
 */
const float* P3M_inv(const float* m, float* mwork);

/**
 * @brief Create a rotation matrix about a given axis
 * 
 * @param angle [in] rotation angle which direction is same as right-handed screw
 * @param p0 [in] axis reference point
 * @param dir [in] axis direction vector
 * @param mwork [in] work area of 4x4 matrix is converted to const float* to return
 * @return const float* 4x4 homogeneous matrix
 */
const float* P3M_rotateaboutaxis(float angle, const float* p0, const float* dir, float* mwork);

/**
 * @brief Camera position relative to default position. The default position is defined as
 * OpenGL defines. i.e. upward vector = Y-axis, positive, look-at vector = Z-axis, negative,
 * camera origin is camera coordinate system origin.
 */
typedef struct {
    float rotAng; // rotation angle about optical axis, rotation angle direction is right-handed screw
    float position[4]; // P3V coordinate in the world
    float lookat[4]; // P3V coordinate in the world; i.e. normalize(lookat - position) is lookat-direction
} P3MCameraPosition_t, *pP3MCameraPosition_t;
typedef const P3MCameraPosition_t *pcP3MCameraPosition_t;

/**
 * @brief Calculate world coordinate to camera local coordinate conversion matrix.
 * 
 * @param cameraposition [in]
 * @param mwork [in] work area of 4x4 matrix is converted to const float* to return
 * @return const float* 4x4 homogeneous matrix
 */
const float* P3M_tocameracoord(pcP3MCameraPosition_t cameraposition, float* mwork);

/**
 * @brief Calculate a transformation matrix for a camera to its scene
 * 
 * @param cameraposition [in] destination and final attitude
 * @param mwork [in] work area of 4x4 matrix is converted to const float* to return
 * @return const float* 4x4 homogeneous matrix
 */
const float* P3M_cameratransform(pcP3MCameraPosition_t cameraposition, float* mwork);

/**
 * @brief invert matA which is a projection matrix in P2 space; i.e. 3x3 matrix
 * 
 * @param matA [in]
 * @param mwork [in] work area to convert to return return pointer
 * @return const float* inv(matA)
 */
const float* P2M_inv(const float* matA, float* mwork);

/**
 * @brief calculate product of 3x3 matrix and 3x1 column vectror
 * 
 * @param matA [in] 3x3 matrix
 * @param vectorB [in] 3x1 column vector
 * @param vwork [in] work area to convert t return pointer
 * @return const float* is a resulted 3x1 column vector
 */
const float* P2MP2V_mult(const float* matA, const float* vectorB, float* vwork);

/**
 * @brief multiply two P2 matrices
 * 
 * @param matA [in] 3x3 matrix
 * @param matB [in] 3x3 matrix
 * @param mwork [in] work area for 3x3 matrix to convert to return pointer
 * @return const float* product matA * matB
 */
const float* P2M_mult(const float* matA, const float* matB, float* mwork);

/**
 * @brief project P3V vector to P2V vector multiplying by 3x4 matrix
 * 
 * @param mat [in] 3x4 matrix
 * @param vector [in] vector in 3-D projection space
 * @param vwork [in] work area to convert to 3x1 column vector
 * @return const float* vector in 2-D projection space
 */
const float* P3VP2V_project(const float* mat, const float* vector, float* vwork);

#define P3M_print(_out_, _caption_, _m_) if (_out_) { \
    fprintf(_out_, "%s, P3M:\n\t%f, %f, %f, %f\n\t%f, %f, %f, %f\n\t%f, %f, %f, %f\n\t%f, %f, %f, %f\n", (_caption_), \
        (_m_)[0], (_m_)[1], (_m_)[2], (_m_)[3], (_m_)[4], (_m_)[5], (_m_)[6], (_m_)[7], \
        (_m_)[8], (_m_)[9], (_m_)[10], (_m_)[11], (_m_)[12], (_m_)[13], (_m_)[14], (_m_)[15]); }
#define P2M_print(_out_, _caption_, _m_) if (_out_) { \
    fprintf(_out_, "%s, P2M:\n\t%f, %f, %f\n\t%f, %f, %f\n\t%f, %f, %f\n", (_caption_), \
        (_m_)[0], (_m_)[1], (_m_)[2], (_m_)[3], (_m_)[4], (_m_)[5], (_m_)[6], (_m_)[7], (_m_)[8] ); }
#ifdef __cplusplus
}
#endif
#endif /* PMATRIX_H_ */