#include "Render/mesh.h"
#include "Pspc/Pvector.h"
#include "Pspc/Pmatrix.h"
#include "Pspc/homography.h"
#include "Render/texture.h"
#include <assert.h>
#include <errno.h>
#include <memory.h>
#include "NLSL/NLSLutils.h"
#include "NLSL/NLSLmatrix.h"

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

void Mesh_transform(pcMesh_t src, pMesh_t dst, const float* mat)
{
    assert(src->ntri == dst->ntri);
    assert(src->nvert == dst->nvert);
    memcpy(dst->indices, src->indices, src->ntri * 3 * sizeof(int32_t));
    const float* srcptr = src->vertices;
    float* dstptr = dst->vertices;
    for (int ivert = 0; ivert != src->nvert; ivert++)
    {
        P3MP3V_mult(mat, srcptr, dstptr);
        srcptr += P3VSIZE;
        dstptr += P3VSIZE;
    }
}

int Mesh_cross(pcMesh_t mesh, const float* p0, const float* dir, pMeshCrossInfo_t crossInfo)
{
    int err = EXIT_FAILURE;
    do {
        crossInfo->mesh = mesh;
        const int* indexptr = mesh->indices;
        const int32_t* indices = mesh->indices;
        for (crossInfo->itri = 0; crossInfo->itri != mesh->ntri; crossInfo->itri++)
        {
            const float* intersection = P3Vtriangle_line_intersection(
                mesh->vertices, indices, p0, dir, crossInfo->intersection);
            if (intersection)
            {
                err = EXIT_SUCCESS;
                break;
            }
            indices += 3;
        }
    } while (0);
    return err;
}

void MeshCrossInfo_show(FILE* out, pcMeshCrossInfo_t crossInfo)
{
    fprintf(out, "intersection = { %f, %f, %f, %f }\n",
        crossInfo->intersection[0], crossInfo->intersection[1], crossInfo->intersection[2],
        crossInfo->intersection[3]);
    fprintf(out, "itri = %d\n", crossInfo->itri);
    const int32_t* triIndices = crossInfo->mesh->indices + crossInfo->itri * 3;
    for (int i = 0; i < 3; i++)
    {
        const float* vert = crossInfo->mesh->vertices + P3VSIZE * triIndices[i];
        fprintf(out, "vert[%d] = { %f, %f, %f, %f }\n", i, vert[0], vert[1], vert[2], vert[3]);
    }
    fprintf(out, " --- \n");
}

int MeshTextureMapperConf_new(pMeshTextureMapperConf_t conf, pcMesh_t mesh)
{
    int err = EXIT_SUCCESS;
    do {
        if (conf->nface == mesh->ntri)
        {
            break;
        }
        else if (conf->nface && conf->textureCoords)
        {
            MeshTextureMapperConf_delete(conf);
        }
        if (((float*)NULL) == (conf->textureCoords = (float*)calloc(mesh->ntri * 3 * R2VSIZE, sizeof(float))))
        {
            err = ENOMEM;
            break;
        }
        conf->nface = mesh->ntri;
    } while (0);
    return err;
}

void MeshTextureMapperConf_delete(pMeshTextureMapperConf_t conf)
{
    if (conf && conf->textureCoords)
    {
        free(conf->textureCoords);
        conf->textureCoords = (float*)NULL;
        conf->nface = 0;
    }
}

static void MeshTextureMapper_init(pMeshTextureMapper_t mapper, pcMeshTextureMapperConf_t conf, pcMesh_t mesh,
    const float* texturesize
) {
    static const int hsize = 9;
    for (int iface = 0; iface != mapper->nface; iface++)
    {
        P3TriangleHomology_t homology = {
            { PxV_P3V, { NULL, NULL, NULL }},
            { PxV_P2V, { NULL, NULL, NULL }}
        };
        int* tri = mesh->indices + iface * 3;
        const float *texconf = conf->textureCoords + iface * 6;
        homology.src.vert[0] = &mesh->vertices[P3VSIZE * tri[0]];
        homology.src.vert[1] = &mesh->vertices[P3VSIZE * tri[1]];
        homology.src.vert[2] = &mesh->vertices[P3VSIZE * tri[2]];
        const float realtexturecoord[P2VSIZE * 3] = {
            texturesize[0] * texconf[0], texturesize[1] * texconf[1], 1.0f,
            texturesize[0] * texconf[2], texturesize[1] * texconf[3], 1.0f,
            texturesize[0] * texconf[4], texturesize[1] * texconf[5], 1.0f
        };
        homology.dst.vert[0] = realtexturecoord;
        homology.dst.vert[1] = homology.dst.vert[0] + P2VSIZE;
        homology.dst.vert[2] = homology.dst.vert[1] + P2VSIZE;
        P3TriangleHomology_homographymatrix2(&homology, mapper->h + hsize * iface);
    }
}

int MeshTextureMapper_new(
    pMeshTextureMapper_t mapper, 
    pcMesh_t mesh, 
    pcMeshTextureMapperConf_t conf, 
    pcTextureInterpolator_t texsrc
) {
    int err = EXIT_SUCCESS;
    do {
        if (mapper->h)
        {
            MeshTextureMapper_delete(mapper);
        }
        if (((float*)NULL) == (mapper->h = (float*)calloc(conf->nface * 3 * 3, sizeof(float))))
        {
            err = ENOMEM;
            break;
        }
        mapper->nface = conf->nface;
        mapper->texsrc = texsrc;
        const float texturesize[] = { (float)texsrc->texture->size[0], (float)texsrc->texture->size[1] };
        MeshTextureMapper_init(mapper, conf, mesh, texturesize);
    } while (0);
    return err;
}

void MeshTextureMapper_delete(pMeshTextureMapper_t mapper)
{
    if (mapper && mapper->h)
    {
        free(mapper->h);
        mapper->h = (float*)NULL;
        mapper->nface = 0;
        mapper->texsrc = (pcTextureInterpolator_t)NULL;
    }
}

int MeshTextureMapper_get(pcMeshTextureMapper_t mapper, pcMeshCrossInfo_t crossinfo, float* value)
{
    const float* h = &mapper->h[9 * crossinfo->itri]; // homography matrix to convert mesh vertex 3D coord to real texture coord
    float vtexcoord[3];
    P2MP2V_mult(h, crossinfo->intersection, vtexcoord);
    vtexcoord[0] /= vtexcoord[2];
    vtexcoord[1] /= vtexcoord[2];
    vtexcoord[2] = 1.0f;
    return TextureInterpolator_get(mapper->texsrc, vtexcoord, value);
}