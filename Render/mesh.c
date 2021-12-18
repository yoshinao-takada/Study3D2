#include "Render/mesh.h"
#include "Pspc/Pvector.h"
#include "Pspc/Pmatrix.h"
#include <assert.h>
#include <errno.h>
#include <memory.h>
#include "NLSL/NLSLutils.h"

int Mesh_new(pMesh_t mesh, int nvert, int ntri)
{
    int err = EXIT_SUCCESS;
    do {
        size_t nelements = 
            nvert * P3VSIZE // 3D vertex homogeneous coordinates
            + ntri * 3; // indices of triangle vertices
        float* p = (float*)calloc(nelements, sizeof(float));
        if (!p)
        {
            err = ENOMEM;
            break;
        }
        mesh->ntri = ntri;
        mesh->nvert = nvert;
        mesh->vertices = p;
        p += P3VSIZE * nvert;
        mesh->indices = (int32_t*)p;
    } while (0);
    return err;
}

void Mesh_delete(pMesh_t mesh)
{
    if (mesh->vertices)
    {
        free(mesh->vertices);
        mesh->vertices = (float*)NULL;
        mesh->indices = (int32_t*)NULL;
    }
}

// Get a rotationally symmetrical point of a vertex of a triangle
// P3V_Trivert[0] is ratated against the center of P3V_TriVert[1] and [2] by 180 degrees
// rotation axis is cross(P3V_TriVert[1] - P3V_TriVert[0], P3V_TriVert[2] - P3V_TriVert[0]).
const float* RotationallySymmetricalPoint(const float* P3V_TriVert[], float* P3Vwork)
{
    static const float pi = __f32 (3.141592653589793238462643383279502884);
    float rotcenter[P3VSIZE], rotaxis[P3VSIZE], rotmat[P3MSIZE], vert1_vert0[P3VSIZE], vert2_vert0[P3VSIZE];
    // center of vert[1] and vert[2]
    P3V_add(P3V_TriVert[1], P3V_TriVert[2], rotcenter);
    rotcenter[3] *= 2.0f;

    // rotation axis
    P3V_sub(P3V_TriVert[1], P3V_TriVert[0], vert1_vert0); // vert[1] - vert[0]
    P3V_sub(P3V_TriVert[2], P3V_TriVert[0], vert2_vert0); // vert[2] - vert[0]
    P3V_cross(vert1_vert0, vert2_vert0, rotaxis);

    // create the rotation matrix
    P3M_rotateaboutaxis(pi, rotcenter, rotaxis, rotmat);

    // rotate vert[1]
    return P3MP3V_mult(rotmat, P3V_TriVert[0], P3Vwork);
}

// Prerequisite 1: Assuming triangle vertices numbered CCW 0, 1, 2
// Step 1: make two paralleograms adding vertex 3s by adding a 180 degs rotationary symmetrical point of vertex 1s.
// Step 2: create a homology struct
// Step 3: calculate homography matrix
const float* Mesh_calchomography(
    const int32_t* vertIndices, const float* vertP3, const float* vertTextureP2, float* mwork
) {
    pP2GridHomology_t homologies = NULL;
    int err = EXIT_SUCCESS;
    const float* trivert[3];
    float newvert[P3VSIZE];
    float P3Vtexturework[3 * P3VSIZE];
    const float* retptr = NULL;
    do {
        // Step 1:
        if (EXIT_SUCCESS != (err = P2GridHomology_new(2,2,&homologies)))
        {
            break;
        }

        // Step 2:
        // Step 2.1: make a parallelogram for 3D space vertices
        trivert[0] = vertP3 + P3VSIZE * vertIndices[0];
        trivert[1] = vertP3 + P3VSIZE * vertIndices[1];
        trivert[2] = vertP3 + P3VSIZE * vertIndices[2];
        RotationallySymmetricalPoint(trivert, newvert);
        for (int i = 0; i < 3; i++)
        {
            homologies->points[i].PJ[0] = trivert[i][0];
            homologies->points[i].PJ[1] = trivert[i][1];
            homologies->points[i].PJ[2] = trivert[i][2];
        }
        homologies->points[3].PJ[0] = newvert[0];
        homologies->points[3].PJ[1] = newvert[1];
        homologies->points[3].PJ[2] = newvert[2];

        // Step 2.2: make a parallelo
        memcpy(P3Vtexturework, vertTextureP2 + P2VSIZE * vertIndices[0], P2VSIZE * sizeof(float));
        memcpy(P3Vtexturework + P3VSIZE, vertTextureP2 + P2VSIZE * vertIndices[1], P2VSIZE * sizeof(float));
        memcpy(P3Vtexturework + 2 * P3VSIZE, vertTextureP2 + P2VSIZE * vertIndices[2], P2VSIZE * sizeof(float));
        trivert[0] = P3Vtexturework;
        trivert[1] = trivert[0] + P3VSIZE;
        trivert[2] = trivert[0] + P3VSIZE;
        RotationallySymmetricalPoint(trivert, newvert);
        for (int i = 0; i < 3; i++)
        {
            homologies->points[i].PK[0] = trivert[i][0];
            homologies->points[i].PK[1] = trivert[i][1];
            homologies->points[i].PK[2] = trivert[i][2];
        }
        homologies->points[3].PK[0] = newvert[0];
        homologies->points[3].PK[1] = newvert[1];
        homologies->points[3].PK[2] = newvert[2];

        // Step 3: calculate homography matrix
        retptr = P2GridHomology_homographymatrix(homologies, mwork);
    } while(0);
    return retptr;
}

void Mesh_calc_mP2VertToTexture(pMesh_t mesh)
{
    for (int itri = 0; itri != mesh->ntri; itri++)
    {
        const int32_t *vertIndices = mesh->indices + 3 * itri;
        float* matHomography = mesh->mP2VertToTexture + P3MSIZE * itri;
        Mesh_calchomography(vertIndices, mesh->vertices , mesh->texture, matHomography);
    }
}

int Mesh_nfloats(pcMesh_t mesh)
{
    return mesh->nvert * (P3VSIZE + P2VSIZE);
}

int Mesh_nints(pcMesh_t mesh)
{
    return mesh->ntri * 3;
}

void Mesh_transform(pcMesh_t src, pMesh_t dst, const float* mat)
{
    assert(src->ntri == dst->ntri);
    assert(src->nvert == dst->nvert);
    memcpy(dst->texture, src->texture, P2VSIZE * src->nvert * sizeof(float));
    memcpy(dst->indices, src->indices, src->ntri * 3 * sizeof(int32_t));
    const float* srcptr = src->vertices;
    float* dstptr = dst->vertices;
    for (int ivert = 0; ivert != src->nvert; ivert++)
    {
        P3MP3V_mult(mat, srcptr, dstptr);
        srcptr += P3VSIZE;
        dstptr += P3VSIZE;
    }
    Mesh_calc_mP2VertToTexture(dst); // update conversion matrices
}

int Mesh_cross(pcMesh_t mesh, const float* p0, const float* dir, pMeshCrossInfo_t crossInfo)
{
    int err = EXIT_FAILURE;
    do {
        crossInfo->mesh = mesh;
        crossInfo->indices = mesh->indices;
        const int* indexptr = mesh->indices;
        for (int itri = 0; itri != mesh->ntri; itri++)
        {
            const float* intersection = P3Vtriangle_line_intersection(
                mesh->vertices, crossInfo->indices, p0, dir, crossInfo->intersection);
            if (intersection)
            {
                err = EXIT_SUCCESS;
                break;
            }
            crossInfo->indices += 3;
        }
    } while (0);
    return err;
}

int Mesh_texture(pcMesh_t mesh, pcMeshCrossInfo_t crossInfo, pcTextureInterpolator_t texture, float* value)
{
    int err = EXIT_SUCCESS;
    pP2GridHomology_t textureHomologies = NULL;
    do {
        if (EXIT_SUCCESS != (err = P2GridHomology_new(4, 1, &textureHomologies)))
        {
            break;
        }
    } while (0);
    return err;
}