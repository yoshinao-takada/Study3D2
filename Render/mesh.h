#ifndef MESH_H_
#define MESH_H_
#include "ZnccC/ImageC.h"
#include "Render/texture.h"
#ifdef __cplusplus
#include <cstdio>
extern "C" {
#else
#include <stdio.h>
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
#define NULLMESH    { 0, 0, (float*)NULL, (int32_t*)NULL }

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
    int itri; // indices of triangle vertices
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
 * @brief format and print into a file stream.
 * 
 * @param out [in] output file stream
 * @param crossInfo [in] data to show
 */
void MeshCrossInfo_show(FILE* out, pcMeshCrossInfo_t crossInfo);

typedef struct {
    int nface; // triangle face count
    float* textureCoords; // each face has (3 * R2VSIZE) elements, totally nface * (3 * R2VSIZE) elements
} MeshTextureMapperConf_t, *pMeshTextureMapperConf_t;
typedef const MeshTextureMapperConf_t *pcMeshTextureMapperConf_t;

#define NULL_MESHTEXTUREMAPPERCONF    { 0, (float*)NULL }

typedef struct {
    int nface; // triangle face count
    pcTextureInterpolator_t texsrc;
    float* h; // array of homography matrices (nface * 3 * 3 elements)
} MeshTextureMapper_t, *pMeshTextureMapper_t;
typedef const MeshTextureMapper_t *pcMeshTextureMapper_t;
#define NULL_MESHTEXTUREMAPPER    { 0, (pcTextureInterpolator_t)NULL, (float*)NULL }

/**
 * @brief allocate a memory block to an object
 * 
 * @param conf [in,out] object to initialize
 * @param mesh [in] target mesh to apply a texture
 * @return int 
 */
int MeshTextureMapperConf_new(pMeshTextureMapperConf_t conf, pcMesh_t mesh);

/**
 * @brief free a memory block in an existing object
 * 
 * @param conf 
 */
void MeshTextureMapperConf_delete(pMeshTextureMapperConf_t conf);

/**
 * @brief allocate a memory block to an object and initialize the internal tables.
 * 
 * @param mapper [in,out] an object to initialize
 * @param mesh [in]
 * @param conf [in]
 * @param texsrc [in]
 * @return int 
 */
int MeshTextureMapper_new(
    pMeshTextureMapper_t mapper, 
    pcMesh_t mesh, 
    pcMeshTextureMapperConf_t conf, 
    pcTextureInterpolator_t texsrc);

/**
 * @brief free a memory block in an existing object
 * 
 * @param mapper 
 */
void MeshTextureMapper_delete(pMeshTextureMapper_t mapper);

/**
 * @brief get a texture color value at a point specified by crossinfo.
 * 
 * @param mapper [in] whole texture info
 * @param crossinfo [in] coordinate info on selected mesh vertices
 * @return int 
 */
int MeshTextureMapper_get(pcMeshTextureMapper_t mapper, pcMeshCrossInfo_t crossinfo, float* value);
#ifdef __cplusplus
}
#endif
#endif /* MESH_H_ */