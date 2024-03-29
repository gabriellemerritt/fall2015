#include "sphere.h"

#include <iostream>

#include <la.h>

static const int SPH_IDX_COUNT = 2280;  // 760 tris * 3
static const int SPH_VERT_COUNT = 382;
float computeParam(const float A, const float B, const float C);
bool rayIntersectionTest (const float A, const float B, const float C);

bool rayIntersectionTest(const float A, const float B, const float C)
{
    float discrim = pow(B,2) - 4.0f*A*C;
    if(discrim < 0.0f )
    {
        return  false;
    }
    return true;

}

float computeParam(const float A, const float B, const float C)
{
    float t_0 = (-B + sqrt(pow(B,2.0) -4.0f*A*C))/(2*A);
    float t_1  = (-B - sqrt(pow(B,2.0) -4.0f*A*C))/(2*A);

    if( t_0 > t_1)
    {
        float temp = t_0;
        t_0 = t_1;
        t_1 = temp;
    }

    if(t_0 > 0.0f)
    {
        return t_0;
    }
    return t_1;
}

Intersection Sphere::GetIntersection(Ray r) // needs to be transformed probably
{
    float radius = 0.5f;
    float A,B,C;
        Intersection sphereIntersection;

        Ray rt = r.GetTransformedCopy(transform.invT());
        A =pow(rt.direction.x, 2.0) + pow(rt.direction.y,2.0) + pow(rt.direction.z, 2.0);
        B  = 2*(rt.direction.x*(rt.origin.x - 0.0f) + rt.direction.y*(rt.origin.y - 0.0f) + rt.direction.z*(rt.origin.z - 0.0f));
        C = pow((rt.origin.x - 0.0f),2.0) + pow((rt.origin.y - 0.0f),2 ) + pow((rt.origin.z -0.0f),2.0) - pow(0.5f,2.0);
        if ( !rayIntersectionTest(A,B,C))
        {
            sphereIntersection.object_hit = NULL;
            return sphereIntersection;
        }
       float  t  = computeParam(A,B,C);
        sphereIntersection.object_hit = this;

        // convert to world space
        glm::vec3 point_object_space = r.origin + t*r.direction;

        glm::vec4 point_homo = transform.T()*glm::vec4(point_object_space,1); // check transform

//        glm::vec4  norm = transform.T()* glm::vec4(point_object_space.x/radius, point_object_space.y/radius, point_object_space.z/radius, 1);

        glm::vec3 point_w = glm::vec3(point_homo[0], point_homo[1], point_homo[2]);

        glm::vec3 sphereNorm = glm::normalize(point_w);

       float world_t = glm::length(point_w - r.origin);
       sphereIntersection.point = point_w;
       sphereIntersection.t =world_t;
       sphereIntersection.normal = sphereNorm;




    return sphereIntersection;
}

// These are functions that are only defined in this cpp file. They're used for organizational purposes
// when filling the arrays used to hold the vertex and index data.
void createSphereVertexPositions(glm::vec3 (&sph_vert_pos)[SPH_VERT_COUNT])
{
    // Create rings of vertices for the non-pole vertices
    // These will fill indices 1 - 380. Indices 0 and 381 will be filled by the two pole vertices.
    glm::vec4 v;
    // i is the Z axis rotation
    for (int i = 1; i < 19; i++) {
        // j is the Y axis rotation
        for (int j = 0; j < 20; j++) {
            v = glm::rotate(glm::mat4(1.0f), j / 20.f * TWO_PI, glm::vec3(0, 1, 0))
                * glm::rotate(glm::mat4(1.0f), -i / 18.0f * PI, glm::vec3(0, 0, 1))
                * glm::vec4(0, 0.5f, 0, 1);
            sph_vert_pos[(i - 1) * 20 + j + 1] = glm::vec3(v);
        }
    }
    // Add the pole vertices
    sph_vert_pos[0] = glm::vec3(0, 0.5f, 0);
    sph_vert_pos[381] = glm::vec3(0, -0.5f, 0);  // 361 - 380 are the vertices for the bottom cap
}


void createSphereVertexNormals(glm::vec3 (&sph_vert_nor)[SPH_VERT_COUNT])
{
    // Unlike a cylinder, a sphere only needs to be one normal per vertex
    // because a sphere does not have sharp edges.
    glm::vec4 v;
    // i is the Z axis rotation
    for (int i = 1; i < 19; i++) {
        // j is the Y axis rotation
        for (int j = 0; j < 20; j++) {
            v = glm::rotate(glm::mat4(1.0f), j / 20.0f * TWO_PI, glm::vec3(0, 1, 0))
                * glm::rotate(glm::mat4(1.0f), -i / 18.0f * PI, glm::vec3(0, 0, 1))
                * glm::vec4(0, 1.0f, 0, 0);
            sph_vert_nor[(i - 1) * 20 + j + 1] = glm::vec3(v);
        }
    }
    // Add the pole normals
    sph_vert_nor[0] = glm::vec3(0, 1.0f, 0);
    sph_vert_nor[381] = glm::vec3(0, -1.0f, 0);
}


void createSphereIndices(GLuint (&sph_idx)[SPH_IDX_COUNT])
{
    int index = 0;
    // Build indices for the top cap (20 tris, indices 0 - 60, up to vertex 20)
    for (int i = 0; i < 19; i++) {
        sph_idx[index] = 0;
        sph_idx[index + 1] = i + 1;
        sph_idx[index + 2] = i + 2;
        index += 3;
    }
    // Must create the last triangle separately because its indices loop
    sph_idx[57] = 0;
    sph_idx[58] = 20;
    sph_idx[59] = 1;
    index += 3;

    // Build indices for the body vertices
    // i is the Z axis rotation
    for (int i = 1; i < 19; i++) {
        // j is the Y axis rotation
        for (int j = 0; j < 20; j++) {
            sph_idx[index] = (i - 1) * 20 + j + 1;
            sph_idx[index + 1] = (i - 1) * 20 +  j + 2;
            sph_idx[index + 2] = (i - 1) * 20 +  j + 22;
            sph_idx[index + 3] = (i - 1) * 20 +  j + 1;
            sph_idx[index + 4] = (i - 1) * 20 +  j + 22;
            sph_idx[index + 5] = (i - 1) * 20 +  j + 21;
            index += 6;
        }
    }

    // Build indices for the bottom cap (20 tris, indices 2220 - 2279)
    for (int i = 0; i < 19; i++) {
        sph_idx[index] = 381;
        sph_idx[index + 1] = i + 361;
        sph_idx[index + 2] = i + 362;
        index += 3;
    }
    // Must create the last triangle separately because its indices loop
    sph_idx[2277] = 381;
    sph_idx[2278] = 380;
    sph_idx[2279] = 361;
    index += 3;
}

void Sphere::create()
{
    GLuint sph_idx[SPH_IDX_COUNT];
    glm::vec3 sph_vert_pos[SPH_VERT_COUNT];
    glm::vec3 sph_vert_nor[SPH_VERT_COUNT];
    glm::vec3 sph_vert_col[SPH_VERT_COUNT];

    createSphereVertexPositions(sph_vert_pos);
    createSphereVertexNormals(sph_vert_nor);
    createSphereIndices(sph_idx);
    for (int i = 0; i < SPH_VERT_COUNT; i++) {
        sph_vert_col[i] = material->base_color;
    }

    count = SPH_IDX_COUNT;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(sph_idx, SPH_IDX_COUNT * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(sph_vert_pos, SPH_VERT_COUNT * sizeof(glm::vec3));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(sph_vert_col, SPH_VERT_COUNT * sizeof(glm::vec3));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(sph_vert_nor, SPH_VERT_COUNT * sizeof(glm::vec3));
}
