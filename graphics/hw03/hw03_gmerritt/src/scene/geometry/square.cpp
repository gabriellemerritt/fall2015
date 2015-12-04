#include <scene/geometry/square.h>
// THIS IS  A PLANE
Intersection SquarePlane::GetIntersection(Ray r)
{
    float t;
    float len = 1.0f/2.0f;
    Intersection planeIntersection;

    // move transformation out of this function eventually
    Ray rt = r.GetTransformedCopy(transform.invT());
    glm::vec4 normal(0,0,1,0);
    glm::vec3 norm(0,0,1);
    glm::vec3 planePoint = glm::vec3(0.0f,0.0f,0.0f);
    glm::vec4 pP = glm::vec4(planePoint, 1);

    // calculate if it hits square

    t = glm::dot(norm, planePoint - rt.origin)/ glm::dot(norm,rt.direction);
    glm::vec3 intersectionPoint = rt.origin + t*rt.direction;

    if (glm::abs(intersectionPoint.x) > len || glm::abs(intersectionPoint.y) > len || t < 0)
    {
        planeIntersection.object_hit = NULL;
        return planeIntersection;
    }

    // set intersection values
    glm::vec4 calcNom = transform.invTransT()*normal;
    planeIntersection.normal = glm::vec3(calcNom[0], calcNom[1], calcNom[2]);
    planeIntersection.object_hit = this;
    glm::vec4 world_point = transform.T()*pP;
    planeIntersection.point = glm::vec3(world_point[0], world_point[1],world_point[2]);
    planeIntersection.t = glm::length(planeIntersection.point - r.origin);
   //
    planeIntersection.intersectColor = this->material->base_color;
    if(this->material->texture != NULL)
    {
        planeIntersection.intersectColor =
        this->material->GetImageColor(SquarePlane::GetUVCoordinates(intersectionPoint),this->material->texture);
    }

    return planeIntersection;
}
void SquarePlane::setBbox()
{
    float mside = 0.5f;
    float xmax = -INFINITY;
    float xmin = INFINITY;
    float ymax =-INFINITY;
    float ymin = INFINITY;
    float zmax = -INFINITY;
    float zmin = INFINITY;

    std::vector<glm::vec3> max_array;
   max_array.push_back( glm::vec3(this->transform.T()*glm::vec4(mside,mside,0,1.0f)));
   max_array.push_back( glm::vec3(this->transform.T()*glm::vec4(mside,-mside,0,1.0f)));
   max_array.push_back( glm::vec3(this->transform.T()*glm::vec4(-mside,mside,0,1.0f)));
   max_array.push_back(  glm::vec3(this->transform.T()*glm::vec4(-mside,-mside,0,1.0f)));

   for (int i = 0; i < 4; i++)
   {
       xmax =  glm::max(xmax, max_array[i].x);
       ymax =  glm::max(ymax, max_array[i].y);
       zmax =  glm::max(zmax, max_array[i].z);

       xmin = glm::min ( xmin, max_array[i].x);
       ymin = glm::min ( ymin, max_array[i].y);
       zmin = glm::min ( zmin, max_array[i].z);
   }
    glm::vec3 max = glm::vec3(xmax,ymax,zmax);
    glm::vec3 min = glm::vec3(xmin, ymin, zmin);

    this->bb = new BoundingBox(max,min);
    this->bb->center = this->bb->getCenterPoint(min,max);
}

void SquarePlane::create()
{
    setBbox();
    this->bb->create();
    GLuint cub_idx[6];
    glm::vec3 cub_vert_pos[4];
    glm::vec3 cub_vert_nor[4];
    glm::vec3 cub_vert_col[4];

    cub_vert_pos[0] = glm::vec3(-0.5f, 0.5f, 0);  cub_vert_nor[0] = glm::vec3(0, 0, 1); cub_vert_col[0] = material->base_color;
    cub_vert_pos[1] = glm::vec3(-0.5f, -0.5f, 0); cub_vert_nor[1] = glm::vec3(0, 0, 1); cub_vert_col[1] = material->base_color;
    cub_vert_pos[2] = glm::vec3(0.5f, -0.5f, 0);  cub_vert_nor[2] = glm::vec3(0, 0, 1); cub_vert_col[2] = material->base_color;
    cub_vert_pos[3] = glm::vec3(0.5f, 0.5f, 0);   cub_vert_nor[3] = glm::vec3(0, 0, 1); cub_vert_col[3] = material->base_color;

    cub_idx[0] = 0; cub_idx[1] = 1; cub_idx[2] = 2;
    cub_idx[3] = 0; cub_idx[4] = 2; cub_idx[5] = 3;

    count = 6;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(cub_idx, 6 * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(cub_vert_pos, 4 * sizeof(glm::vec3));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(cub_vert_nor, 4 * sizeof(glm::vec3));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(cub_vert_col, 4 * sizeof(glm::vec3));
}

glm::vec2 SquarePlane::GetUVCoordinates(const glm::vec3 &point)
{
   glm::vec2 uv_coords =  glm::vec2(point.x + 0.5f , point.y +0.5f);

   return uv_coords;
}
