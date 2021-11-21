#ifndef PMATRIX_H_
#define PMATRIX_H_
#ifdef __cplusplus
extern "C" {
#endif

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
const float* P3M_rotatewithaxis(float angle, const float* p0, const float* dir, float* mwork);

/**
 * @brief Camera position relative to default position. The default position is defined as
 * OpenGL defines. i.e. upward vector = Y-axis, positive, look-at vector = Z-axis, negative,
 * camera origin is camera coordinate system origin.
 */
typedef struct {
    float rotAng; // rotation angle about optical axis, rotation angle direction is right-handed screw
    float position[4]; // P3V coordinate in the world
    float lookat[4]; // P3V coordinate in the world
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

typedef enum {
    FOVaxis_x,
    FOVaxis_y
} FOVaxis_t;

typedef struct {
    float vpBL[2]; // viewport bottom-left pixel coordinate
    float vpTR[2]; // viewport top-right pixel coordinate
    float fov; // vertical or horizontal angle of field-of-view
    FOVaxis_t fovaxis; // FOV direction
    float sn; // normalized skew factor
} P3MCamera_t, *pP3MCamera_t;
typedef const P3MCamera_t *pcP3MCamera_t;

/**
 * @brief Calculate the conversion matrix from camera local coordinate to viewport
 * Camera local coordinate system
 *  Y+ axis: upward direction
 *  Z- axis: look-at line
 *  O: camera focal point = camera frustum top vertex
 * Reference image plane
 *  d = 1: distance from O
 * 
 * @param m [out] 3x4 homogeneous coordinate conversion matrix, which converts P3 space to P2 plane
 * @param camera [in] camera parameters
 */
const float* P3M_cameraintrinsics(pcP3MCamera_t camera, float* mwork);

#ifdef __cplusplus
}
#endif
#endif /* PMATRIX_H_ */