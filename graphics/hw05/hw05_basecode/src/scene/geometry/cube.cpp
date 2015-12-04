#include <math.h>
#include "cube.h"
#include <la.h>
#include <iostream>
#include <random>
#include <algorithm>
static const int CUB_IDX_COUNT = 36;
static const int CUB_VERT_COUNT = 24;
glm::vec3 randomPoint(float rand1, float rand2);
void Cube::ComputeArea()
{
    //Extra credit to implement this
    area = 2*(transform.getScale().x*transform.getScale().y + transform.getScale().y*transform.getScale().z + transform.getScale().z*transform.getScale().x);
}
 glm::vec3 randomPoint(float rand1, float rand2)
 {
        std::mt19937 rand_factor(rand());
         std::vector<glm::vec3> surfacePoint;
         glm::vec3 xFacep1(0.5f, rand1 , rand2);
         glm::vec3 xFacep2(0.5f,  rand1 , rand2);

         glm::vec3 yFacep1(rand1, 0.5f, rand2);
         glm::vec3 yFacep2(rand2, 0.5f, rand1);

         glm::vec3 zFacep1( rand1 , rand2,0.5f);
         glm::vec3 zFacep2( rand2 , rand1,0.5f);

         surfacePoint.push_back(xFacep1);
         surfacePoint.push_back(-xFacep2);
         surfacePoint.push_back(yFacep1);
         surfacePoint.push_back(-yFacep2);
         surfacePoint.push_back(zFacep1);
         surfacePoint.push_back(-zFacep2);
         std::random_shuffle(surfacePoint.begin(), surfacePoint.end());

         return surfacePoint.at(1);

 }

glm::vec3 Cube::findBiTangent(const glm::vec3 &normal)
{
         std::mt19937 rand_factor(rand());
         std::uniform_real_distribution<float> uni_dis(-0.5f,0.5f);
        glm::vec3 xFacep1(0.5f, uni_dis(rand_factor), uni_dis(rand_factor));
        glm::vec3 xFacep2(0.5f, uni_dis(rand_factor), uni_dis(rand_factor));

        glm::vec3 yFacep1(uni_dis(rand_factor), 0.5f, uni_dis(rand_factor));
        glm::vec3 yFacep2(uni_dis(rand_factor), 0.5f, uni_dis(rand_factor));

        glm::vec3 zFacep1(uni_dis(rand_factor), uni_dis(rand_factor),0.5f);
        glm::vec3 zFacep2(uni_dis(rand_factor), uni_dis(rand_factor),0.5f);
        float min = -1;
        float max =1;
      glm::vec3 bitangent;
      if(fequal(normal.x, max )){
          bitangent = CalculateBiTan(normal/2.0f, xFacep1, xFacep2);
      }
      if(fequal(normal.x, min)){
          bitangent =  CalculateBiTan(normal/2.0f, -xFacep1, -xFacep2);
      }
      if(fequal(normal.y, max)){
          bitangent =  CalculateBiTan(normal/2.0f, yFacep1, yFacep2);
      }
      if(fequal(normal.y, min)){
         bitangent =  CalculateBiTan(normal/2.0f, -yFacep1, -yFacep2);
      }
      if(fequal(normal.z, max)){
          bitangent =  CalculateBiTan(normal/2.0f, zFacep1, zFacep2);
      }
      if(fequal(normal.z, min)){
          bitangent =  CalculateBiTan(normal/2.0f, -zFacep1, -zFacep2);
      }

      return bitangent;
}
glm::vec3 Cube::findTangent(const glm::vec3 &normal)
{
    std::mt19937 rand_factor(rand());
    std::uniform_real_distribution<float> uni_dis(-0.5f,0.5f);
        glm::vec3 xFacep1(0.5f, uni_dis(rand_factor), uni_dis(rand_factor));
        glm::vec3 xFacep2(0.5f, uni_dis(rand_factor), uni_dis(rand_factor));

        glm::vec3 yFacep1(uni_dis(rand_factor), 0.5f, uni_dis(rand_factor));
        glm::vec3 yFacep2(uni_dis(rand_factor), 0.5f, uni_dis(rand_factor));

        glm::vec3 zFacep1(uni_dis(rand_factor), uni_dis(rand_factor),0.5f);
        glm::vec3 zFacep2(uni_dis(rand_factor), uni_dis(rand_factor),0.5f);
        float min = -1;
        float max =1;
      glm::vec3 tangent;
      if(fequal(normal.x, max )){
          tangent = CalculateTangent(normal/2.0f, xFacep1, xFacep2);
      }
      if(fequal(normal.x, min)){
          tangent =  CalculateTangent(normal/2.0f, -xFacep1, -xFacep2);
      }
      if(fequal(normal.y, max)){
          tangent =  CalculateTangent(normal/2.0f, yFacep1, yFacep2);
      }
      if(fequal(normal.y, min)){
          tangent =  CalculateTangent(normal/2.0f, -yFacep1, -yFacep2);
      }
      if(fequal(normal.z, max)){
          tangent =  CalculateTangent(normal/2.0f, zFacep1, zFacep2);
      }
      if(fequal(normal.z, min)){
          tangent =  CalculateTangent(normal/2.0f, -zFacep1, -zFacep2);
      }

      return tangent;
}

glm::vec4 GetCubeNormal(const glm::vec4& P)
{
    int idx = 0;
    float val = -1;
    for(int i = 0; i < 3; i++){
        if(glm::abs(P[i]) > val){
            idx = i;
            val = glm::abs(P[i]);
        }
    }
    glm::vec4 N(0,0,0,0);
    N[idx] = glm::sign(P[idx]);
    return N;
}

glm::vec3 Cube::ComputeNormal(const glm::vec3 &P)
{return glm::vec3(0);}

Intersection Cube::RandomSample(float randX, float randY)
{
    Intersection isx;
    float rX = randX -0.5f;
    float rY = randY -0.5f;
    glm::vec3 rand_point  = randomPoint(rX, rY);
    glm::vec4 isx_point = glm::vec4(rand_point ,1);
    isx.point = glm::vec3(transform.T()*isx_point);
    isx.object_hit = this;
    isx.normal = glm::normalize(glm::vec3(transform.invTransT() * glm::vec4(ComputeNormal(glm::vec3(isx_point)), 0)));
    isx.texture_color = Material::GetImageColorInterp(GetUVCoordinates(glm::vec3(isx_point)), material->texture);


    return isx;
}

Intersection Cube::GetIntersection(Ray r)
{
    //Transform the ray
    Ray r_loc = r.GetTransformedCopy(transform.invT());
    Intersection result;

    float t_n = -1000000;
    float t_f = 1000000;
    for(int i = 0; i < 3; i++){
        //Ray parallel to slab check
        if(r_loc.direction[i] == 0){
            if(r_loc.origin[i] < -0.5f || r_loc.origin[i] > 0.5f){
                return result;
            }
        }
        //If not parallel, do slab intersect check
        float t0 = (-0.5f - r_loc.origin[i])/r_loc.direction[i];
        float t1 = (0.5f - r_loc.origin[i])/r_loc.direction[i];
        if(t0 > t1){
            float temp = t1;
            t1 = t0;
            t0 = temp;
        }
        if(t0 > t_n){
            t_n = t0;
        }
        if(t1 < t_f){
            t_f = t1;
        }
    }
    float t_final = -1;
    if(t_n < t_f)
    {
        if(t_n >= 0)
        {
            t_final = t_n;
        }
        else if(t_f >= 0)
        {
            t_final = t_f;
        }
    }
    if(t_final >= 0)
    {
        //Lastly, transform the point found in object space by T
        glm::vec4 P = glm::vec4(r_loc.origin + t_final*r_loc.direction, 1);
        result.point = glm::vec3(transform.T() * P);
        result.normal = glm::normalize(glm::vec3(transform.invTransT() * GetCubeNormal(P)));
        result.object_hit = this;
        result.t = glm::distance(result.point, r.origin);
        result.texture_color = Material::GetImageColorInterp(GetUVCoordinates(glm::vec3(P)), material->texture);
        //TODO: Store the tangent and bitangent
       glm::vec3 obj_tan = findTangent(glm::vec3(GetCubeNormal(P)));
       glm::vec3 obj_bitan = findBiTangent(glm::vec3(GetCubeNormal(P)));
       result.tangent = glm::normalize(glm::vec3(transform.T()*glm::vec4(obj_tan,0)));
       result.bitangent = glm::normalize(glm::vec3(transform.T()*glm::vec4(obj_bitan,0)));
        return result;
    }
    else{
        return result;
    }
}

glm::vec2 Cube::GetUVCoordinates(const glm::vec3 &point)
{
    glm::vec3 abs = glm::min(glm::abs(point), 0.5f);
    glm::vec2 UV;//Always offset lower-left corner
    if(abs.x > abs.y && abs.x > abs.z)
    {
        UV = glm::vec2(point.z + 0.5f, point.y + 0.5f)/3.0f;
        //Left face
        if(point.x < 0)
        {
            UV += glm::vec2(0, 0.333f);
        }
        else
        {
            UV += glm::vec2(0, 0.667f);
        }
    }
    else if(abs.y > abs.x && abs.y > abs.z)
    {
        UV = glm::vec2(point.x + 0.5f, point.z + 0.5f)/3.0f;
        //Left face
        if(point.y < 0)
        {
            UV += glm::vec2(0.333f, 0.333f);
        }
        else
        {
            UV += glm::vec2(0.333f, 0.667f);
        }
    }
    else
    {
        UV = glm::vec2(point.x + 0.5f, point.y + 0.5f)/3.0f;
        //Left face
        if(point.z < 0)
        {
            UV += glm::vec2(0.667f, 0.333f);
        }
        else
        {
            UV += glm::vec2(0.667f, 0.667f);
        }
    }
    return UV;
}

//These are functions that are only defined in this cpp file. They're used for organizational purposes
//when filling the arrays used to hold the vertex and index data.
void createCubeVertexPositions(glm::vec3 (&cub_vert_pos)[CUB_VERT_COUNT])
{
    int idx = 0;
    //Front face
    //UR
    cub_vert_pos[idx++] = glm::vec3(0.5f, 0.5f, 0.5f);
    //LR
    cub_vert_pos[idx++] = glm::vec3(0.5f, -0.5f, 0.5f);
    //LL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, -0.5f, 0.5f);
    //UL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, 0.5f, 0.5f);

    //Right face
    //UR
    cub_vert_pos[idx++] = glm::vec3(0.5f, 0.5f, -0.5f);
    //LR
    cub_vert_pos[idx++] = glm::vec3(0.5f, -0.5f, -0.5f);
    //LL
    cub_vert_pos[idx++] = glm::vec3(0.5f, -0.5f, 0.5f);
    //UL
    cub_vert_pos[idx++] = glm::vec3(0.5f, 0.5f, 0.5f);

    //Left face
    //UR
    cub_vert_pos[idx++] = glm::vec3(-0.5f, 0.5f, 0.5f);
    //LR
    cub_vert_pos[idx++] = glm::vec3(-0.5f, -0.5f, 0.5f);
    //LL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, -0.5f, -0.5f);
    //UL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, 0.5f, -0.5f);

    //Back face
    //UR
    cub_vert_pos[idx++] = glm::vec3(-0.5f, 0.5f, -0.5f);
    //LR
    cub_vert_pos[idx++] = glm::vec3(-0.5f, -0.5f, -0.5f);
    //LL
    cub_vert_pos[idx++] = glm::vec3(0.5f, -0.5f, -0.5f);
    //UL
    cub_vert_pos[idx++] = glm::vec3(0.5f, 0.5f, -0.5f);

    //Top face
    //UR
    cub_vert_pos[idx++] = glm::vec3(0.5f, 0.5f, -0.5f);
    //LR
    cub_vert_pos[idx++] = glm::vec3(0.5f, 0.5f, 0.5f);
    //LL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, 0.5f, 0.5f);
    //UL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, 0.5f, -0.5f);

    //Bottom face
    //UR
    cub_vert_pos[idx++] = glm::vec3(0.5f, -0.5f, 0.5f);
    //LR
    cub_vert_pos[idx++] = glm::vec3(0.5f, -0.5f, -0.5f);
    //LL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, -0.5f, -0.5f);
    //UL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, -0.5f, 0.5f);
}


void createCubeVertexNormals(glm::vec3 (&cub_vert_nor)[CUB_VERT_COUNT])
{
    int idx = 0;
    //Front
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec3(0,0,1);
    }
    //Right
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec3(1,0,0);
    }
    //Left
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec3(-1,0,0);
    }
    //Back
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec3(0,0,-1);
    }
    //Top
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec3(0,1,0);
    }
    //Bottom
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec3(0,-1,0);
    }
}

void createCubeIndices(GLuint (&cub_idx)[CUB_IDX_COUNT])
{
    int idx = 0;
    for(int i = 0; i < 6; i++){
        cub_idx[idx++] = i*4;
        cub_idx[idx++] = i*4+1;
        cub_idx[idx++] = i*4+2;
        cub_idx[idx++] = i*4;
        cub_idx[idx++] = i*4+2;
        cub_idx[idx++] = i*4+3;
    }
}

void Cube::create()
{
    GLuint cub_idx[CUB_IDX_COUNT];
    glm::vec3 cub_vert_pos[CUB_VERT_COUNT];
    glm::vec3 cub_vert_nor[CUB_VERT_COUNT];
    glm::vec3 cub_vert_col[CUB_VERT_COUNT];

    createCubeVertexPositions(cub_vert_pos);
    createCubeVertexNormals(cub_vert_nor);
    createCubeIndices(cub_idx);

    for(int i = 0; i < CUB_VERT_COUNT; i++){
        cub_vert_col[i] = material->base_color;
    }

    count = CUB_IDX_COUNT;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(cub_idx, CUB_IDX_COUNT * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(cub_vert_pos,CUB_VERT_COUNT * sizeof(glm::vec3));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(cub_vert_nor, CUB_VERT_COUNT * sizeof(glm::vec3));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(cub_vert_col, CUB_VERT_COUNT * sizeof(glm::vec3));

}
