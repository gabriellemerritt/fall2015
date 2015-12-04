#include "cube.h"
#include <la.h>
#include <iostream>

static const int CUB_IDX_COUNT = 36;
static const int CUB_VERT_COUNT = 24;
float compareNearSwap( float t_small, float t_compare);
float compareFarSwap(float t_1, float t_far);
//const float findFarNear(float t_0, float t_1, float t_near, float t_far);

float compareNearSwap(float t_small, float t_compare)
{
    if(t_small > t_compare)
    {
        return t_small;
    }
    return t_compare;
}

float compareFarSwap(float t_1, float t_far)
{
    if(t_1 < t_far)
    {
        return t_1;
    }
    return t_far;
}

Intersection Cube::GetIntersection(Ray r)
{
    Intersection cubeIntersection;
    glm::mat3 normalFace;
    glm::vec3 xFace(1.0f, 0, 0);
    glm::vec3 yFace(0, 1.0f, 0);
    glm::vec3 zFace(0,0,1.0f);

    normalFace[0] = xFace;
    normalFace[1] = yFace;
    normalFace[2] = zFace;
    float min = -0.5f;
    float max = 0.5f;
    float t_0, t_1, t_near, t_far;
//    static_assert(std::numeric_limits<float>::is_iec559, "IEEE 754 required");
    t_near = -INFINITY;
    t_far = INFINITY;
    // transform ray into object space
    Ray rt = r.GetTransformedCopy(transform.invT());

    //calculate t0, t1 for all planes
    for (int i = 0; i < 3; i++)
    {
        if(fequal(rt.direction[i],0.0f));
        {
           if(rt.origin[i] < min  || rt.origin[i] > max)
           {
               cubeIntersection.object_hit = NULL;
           }
        }
        t_0 = (min - rt.origin[i])/ rt.direction[i];
        t_1 = (max - rt.origin[i])/ rt.direction[i];

        if(t_0 > t_1)
        {
            float swap = t_0;
            t_0 = t_1;
            t_1 = swap;

        }
//        if( t_0 < t_near)
//        {
//            cubeIntersection.normal = normalFace[i];
//        }
        t_near = compareNearSwap(t_0,t_near);
        t_far  = compareFarSwap(t_1, t_far);
    }
    if(t_near > t_far || t_near < 0 || t_far < 0)
    {
        cubeIntersection.object_hit = NULL;
        return cubeIntersection;
    }

    // homogenous coordinates for point of intersection in object space
     cubeIntersection.object_hit = this;
    glm::vec4 object_point = glm::vec4((rt.origin + t_near* rt.direction),1);
    glm::vec4 obj_world_point = transform.T()*object_point;
    glm::vec3 point (obj_world_point.x, obj_world_point.y, obj_world_point.z);

      if(fequal(object_point.x, max )){ cubeIntersection.normal =xFace; }
      if(fequal(object_point.x, min)){cubeIntersection.normal = -xFace;}
      if(fequal(object_point.y, max)){cubeIntersection.normal = yFace;}
      if(fequal(object_point.y, min)){cubeIntersection.normal = -yFace;}
      if(fequal(object_point.z, max)){cubeIntersection.normal = zFace;}
      if(fequal(object_point.z, min)){cubeIntersection.normal = -zFace;}

    cubeIntersection.t = glm::length(point - r.origin);
    cubeIntersection.point = point;
    //
    if(this->material->texture != NULL)
    {
        cubeIntersection.intersectColor = this->material->GetImageColor(Cube::GetUVCoordinates(glm::vec3(object_point)), this->material->texture);

    }
    else
    {
        cubeIntersection.intersectColor = this->material->base_color;
    }
   //
    return cubeIntersection;
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

void Cube::setBbox()
{
    float mside = 0.5f;
    float xmax = -INFINITY;
    float xmin = INFINITY;
    float ymax =-INFINITY;
    float ymin = INFINITY;
    float zmax = -INFINITY;
    float zmin = INFINITY;

    std::vector<glm::vec3> max_array;
   max_array.push_back( glm::vec3(this->transform.T()*glm::vec4(mside,mside,mside,1.0f)));
   max_array.push_back(  glm::vec3(this->transform.T()*glm::vec4(mside,mside,-mside,1.0f)));
   max_array.push_back( glm::vec3(this->transform.T()*glm::vec4(mside,-mside,mside,1.0f)));
   max_array.push_back( glm::vec3(this->transform.T()*glm::vec4(-mside,mside,mside,1.0f)));
   max_array.push_back(glm::vec3(this->transform.T()*glm::vec4(mside,-mside,-mside,1.0f)));
   max_array.push_back(  glm::vec3(this->transform.T()*glm::vec4(-mside,-mside,mside,1.0f)));
   max_array.push_back(glm::vec3(this->transform.T()*glm::vec4(-mside,mside,-mside,1.0f)));
   max_array.push_back( glm::vec3(this->transform.T()*glm::vec4(-mside,-mside,-mside,1.0f)));

   for (int i = 0; i < 8; i++)
   {
       xmax =  glm::max(xmax, max_array[i].x);
       ymax =  glm::max(ymax, max_array[i].y);
       zmax =  glm::max(zmax, max_array[i].z);

       xmin = glm::min (xmin, max_array[i].x);
       ymin = glm::min (ymin, max_array[i].y);
       zmin = glm::min (zmin, max_array[i].z);
   }
    glm::vec3 max = glm::vec3(xmax,ymax,zmax);
    glm::vec3 min = glm::vec3(xmin, ymin, zmin);
    this->bb = new BoundingBox(max,min);
    this->bb->center = this->bb->getCenterPoint(min,max);
}

void Cube::create()
{
    setBbox();
    this->bb->create();
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

glm::vec2 Cube::GetUVCoordinates(const glm::vec3 &point)
{
    float min = -0.5f;
    float max = 0.5f;
   glm::vec2 uv_coords = glm::vec2(0,0);
    if (fequal(point.x, max ))
    {  // .5 -- .75 u axis .33 ->,66 v axis
          uv_coords = glm::vec2((glm::abs(point.z -  max )* 0.25f + max), (point.y + max) *0.33f + 0.33f) ;
    }
    if(fequal(point.x, min))
    {
        // 0 - .25 u axis .33 -> .66 v axis
         uv_coords= glm::vec2( ((point.z +  max )* 0.25f ), (point.y + max) *0.33f + 0.33f) ;
    }

    if(fequal(point.y, max))
    {
         uv_coords= glm::vec2( ((point.x +  max )* 0.25f + 0.25f), glm::abs(point.z - max) *0.3333f + 0.66667f);
    }

    if(fequal(point.y, min))
    {
        uv_coords= glm::vec2( ((point.x +  max )* 0.25f + 0.25f), (point.z + max) *0.3333f );

    }
    if(fequal(point.z, max))
    {
         uv_coords= glm::vec2( ((point.x +  max )* 0.25f + 0.25f), (point.y + max) *0.33f + 0.33f) ;
    }

    if(fequal(point.z, min))
    {
         uv_coords= glm::vec2(( glm::abs(point.z -  max )* 0.25f + 0.75f ), (point.y + max) *0.33f + 0.33f) ;
    }

    return uv_coords;
}
