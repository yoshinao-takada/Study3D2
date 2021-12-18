#ifndef PCAMERA_H_
#define PCAMERA_H_
#ifdef __cplusplus
extern "C" {
#endif

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
 * @brief viewport width
 * 
 * @param camera [in]
 * @return float 
 */
float vpwidth(pcP3MCamera_t camera);

/**
 * @brief viewport height
 * 
 * @param camera [in]
 * @return float 
 */
float vpheight(pcP3MCamera_t camera);

/**
 * @brief ratio viewport width / viewport height
 * 
 * @param camera [in]
 * @return float 
 */
float vpwh(pcP3MCamera_t camera);

/**
 * @brief ratio viewport height / viewport width
 * 
 * @param camera [in]
 * @return float 
 */
float vphw(pcP3MCamera_t camera);

/**
 * @brief horizontal FOV angle
 * 
 * @param camera [in]
 * @return float FOV in radian
 */
float fovx(pcP3MCamera_t camera);

/**
 * @brief vertical FOV angle
 * 
 * @param camera [in]
 * @return float FOV in radian
 */
float fovy(pcP3MCamera_t camera);

/**
 * @brief horizontal 0-to-edge FOV angle
 * 
 * @param camera [in]
 * @return float half FOV in radian
 */
float half_fovx(pcP3MCamera_t camera);

/**
 * @brief vertical 0-to-edge FOV angle
 * 
 * @param camera [in]
 * @return float half FOV in radian
 */
float half_fovy(pcP3MCamera_t camera);

/**
 * @brief Calculate the conversion matrix from camera local coordinate to viewport
 * Camera local coordinate system
 *  Y+ axis: upward direction
 *  Z- axis: look-at line
 *  O: camera focal point = camera frustum top vertex
 * Reference image plane
 *  d = 1: distance from O
 * 
 * @param mwork [in] 3x4 homogeneous coordinate conversion matrix, which converts P3 space to P2 plane
 * @param camera [in] camera parameters
 */
const float* P3M_cameraintrinsics(pcP3MCamera_t camera, float* mwork);

typedef struct {
    float PJ[3]; //!< transformed R2 coordinate
    float PK[3]; //!< source R2 coordinate
} P2PointHomology_t, *pP2PointHomology_t;
typedef const P2PointHomology_t *pcP2PointHomology_t;

typedef struct {
    int GridSize[2];
    P2PointHomology_t points[0];
} P2GridHomology_t, *pP2GridHomology_t;
typedef const P2GridHomology_t *pcP2GridHomology_t;

/**
 * @brief Create a new instance of P2GridHomology_t.
 * 
 * @param xsize [in]
 * @param ysize [in]
 * @param ppobj [out]
 * @return int EXIT_SUCCESS: success, ENOMEM: fail in malloc()
 */
int P2GridHomology_new(int xsize, int ysize, pP2GridHomology_t* ppobj);

/**
 * @brief get a pointer to an element at the selected point
 * 
 * @param grid [in] grid homology object
 * @param xindex [in] horizontal element selector
 * @param yindex [in] vertical element selector
 * @return pP2PointHomology_t is an element pointer
 */
pP2PointHomology_t P2GridHomology_point(pP2GridHomology_t grid, int xindex, int yindex);

/**
 * @brief get a const pointer to an element at the selected point
 * 
 * @param grid [in] grid homology object
 * @param xindex [in] horizontal element selector
 * @param yindex [in] vertical element selector
 * @return pcP2PointHomology_t is a const element pointer
 */
pcP2PointHomology_t P2GridHomology_cpoint(pcP2GridHomology_t grid, int xindex, int yindex);

/**
 * @brief create 3x3 homogeneous matrix converting from Pr to Pp.
 * 
 * @param grid [in] point-to-point homology on grid points
 * @return const float* 3x3 matrix
 */
const float* P2GridHomology_homographymatrix(pcP2GridHomology_t grid, float* mwork);
#ifdef __cplusplus
}
#endif
#endif /* PCAMERA_H_ */