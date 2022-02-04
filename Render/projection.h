#ifndef PROJECTION_H_
#define PROJECTION_H_
#include "Render/texture.h"
#include "Render/mesh.h"
#include "Pspc/Pcamera0.h"
#include "Pspc/Pmatrix.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    pcMesh_t geometryModel;
    pcImageC_t texture;
    pcMeshTextureMapperConf_t texConf;
    pcCamera35mmConf_t cameraConf;
    pcP3MCameraPosition_t cameraPosition;
} Scene_t, *pScene_t;
typedef const Scene_t *pcScene_t;

/**
 * @brief render a scene
 * 
 * @param scene [in] definition of the scene
 * @param viewport [out] image which the rendered scene is drawn.
 * @return int EXIT_SUCCESS or unix errno compatible number
 */
int Projection_render(pcScene_t scene, pImageC_t viewport);

typedef struct {
    pcMesh_t geometryModel;
    pcImageC_t texture;
    pcMeshTextureMapperConf_t texConf;
    float transform[P3MSIZE];
} SceneObject_t, *pSceneObject_t;
typedef const SceneObject_t *pcSceneObject_t;

typedef struct {
    int objCount;
    pSceneObject_t objects;
    pcCamera35mmConf_t cameraConf;
    pcP3MCameraPosition_t cameraPosition;
} Scene2_t, *pScene2_t;
typedef const Scene2_t *pcScene2_t;

/**
 * @brief render a scene which contains multiple objects
 * 
 * @param scene 
 * @param viewport 
 * @return int 
 */
int Projection_render2(pcScene2_t scene, pImageC_t viewport);

/**
 * @brief helper function inside Projection_render2() to create instantiated mesh array
 * 
 * @param scene [in]
 * @return pMesh_t containing instantiated meshes
 */
pMesh_t Projection_instantiate(pcScene2_t scene);

/**
 * @brief helper function inside Projection_render2() to create TextureMapper objects
 * 
 * @param scene [in]
 * @param meshes [in]
 * @return pMeshTextureMapper_t containing initialized texture mappers
 */
pMeshTextureMapper_t Projection_textureMapper(pcScene2_t scene, pcMesh_t meshes);
#ifdef __cplusplus
}
#endif
#endif /* PROJECTION_H_ */