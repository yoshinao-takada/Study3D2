#ifndef PROJECTION_H_
#define PROJECTION_H_
#include "Render/texture.h"
#include "Render/mesh.h"
#include "Pspc/Pcamera.h"
#include "Pspc/Pmatrix.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    pcMesh_t geometryModel;
    pcImageC_t texture;
    P3MCameraPosition_t cameraPosition;
    pcP2GridHomology_t gridHomology;
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
#ifdef __cplusplus
}
#endif
#endif /* PROJECTION_H_ */