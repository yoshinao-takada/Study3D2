#ifndef MESH_H_
#define MESH_H_
#include "ZnccC/ImageC.h"
#include "Render/texture.h"
#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief mesh defines data structures and APIs to describe 3-D object consisting of triangle surfaces.
 * The APIs consists of
 * int Mesh_new(): create a new mesh object,
 * void Mesh_calc_mP2VertToTexture() : 
 */
typedef struct {
    int nvert; // vertex count
    int ntri; // triangle count
    float* vertices; // vertex coords; P3 vector
    int32_t* indices; // triangle indices aggregated int[3] x ntri arrays
} Mesh_t, *pMesh_t;
typedef const Mesh_t *pcMesh_t;

/**
 * @brief create a new mesh object
 * 
 * @param mesh 
 * @param nvert 
 * @param ntri 
 * @return int EXIT_SUCCESS or ENOMEM
 */
int Mesh_new(pMesh_t mesh, int nvert, int ntri);

/**
 * @brief release a memory block allocated to mesh
 * 
 * @param mesh 
 */
void Mesh_delete(pMesh_t mesh);

typedef struct {
    pcImageC_t textureSource;
    float* normalizedTextureCoords; // (0, 0) - (1, 1) => (0, 0) - (width, source) normalized coords
    float* hmat; // homography matrices mapping mesh triangle emements to triangles on real texture coordinates
} TextureMapper_t, *pTextureMapper_t;
typedef const TextureMapper_t *pcTextureMapper_t;

/**
 * @brief Update conversion matrices
 * 
 * @param mesh 
 */
void Mesh_calc_mP2VertToTexture(pMesh_t mesh);

/**
 * @brief get a number of float variables; i.e. sum of vertices and texcoords float variables
 * 
 * @param mesh [in]
 * @return int number of float variables
 */
int Mesh_nfloats(pcMesh_t mesh);

/**
 * @brief get a number of int variables; i.e. sum of indices int variables
 * 
 * @param mesh [in]
 * @return int number of int variables
 */
int Mesh_nints(pcMesh_t mesh);

/**
 * @brief transform a mesh
 * 
 * @param src [in] source mesh
 * @param dst [out] destination mesh which must be allocated by caller.
 * @param mat [in] P3 space homogeneous matrix transforming the mesh
 */
void Mesh_transform(pcMesh_t src, pMesh_t dst, const float* mat);

typedef struct {
    float intersection[4]; // homogeneous coordinate of intersection
    const int* indices; // indices of triangle vertices
    pcMesh_t mesh;
} MeshCrossInfo_t, *pMeshCrossInfo_t;
typedef const MeshCrossInfo_t *pcMeshCrossInfo_t;

/**
 * @brief check if a mesh and a line cross.
 * 
 * @param mesh [in]
 * @param p0 [in] reference point of the line
 * @param dir [in] direction vector of the line, which need not be normalized.
 * @param crossInfo [out] info of intersection
 * @return int EXIT_SUCCESS: an intersection was found. EXIT_FAILURE: no intersection
 */
int Mesh_cross(pcMesh_t mesh, const float* p0, const float* dir, pMeshCrossInfo_t crossInfo);

/**
 * @brief get a texture value at the cross point
 * 
 * @param mesh [in]
 * @param crossInfo [in]
 * @param texture [in]
 * @param value [out]
 * @return int EXIT_SUCCESS or errno compatible error code
 */
int Mesh_texture(pcMesh_t mesh, pcMeshCrossInfo_t crossInfo, pcTextureInterpolator_t texture, float* value);

#ifdef __cplusplus
}
#endif
#endif /* MESH_H_ */