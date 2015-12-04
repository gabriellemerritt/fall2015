#include <scene/geometry/square.h>

void SquarePlane::ComputeArea()
{
    float a = transform.getScale().x ;
    float b = transform.getScale().y;
     area = a*b;
}


Intersection SquarePlane::RandomSample(float randX, float randY)
{
    Intersection isx;
    glm::vec3 rand_point  = glm::vec3(randX -0.5f, randY -0.5f, 0);
    glm::vec4 isx_point = glm::vec4(rand_point ,1);

    isx.point = glm::vec3(transform.T()*isx_point);
    isx.object_hit = this;
    isx.normal = glm::normalize(glm::vec3(transform.invTransT() * glm::vec4(ComputeNormal(glm::vec3(isx_point)), 0)));
    isx.texture_color = Material::GetImageColorInterp(GetUVCoordinates(glm::vec3(isx_point)), material->texture);

    //I need to figure out how to do this for intersection points
//    isx.tangent = CalculateTangent(glm::vec3(-0.5f, -0.5f, 0), glm::vec3(-0.5f,0.5f,0), glm::vec3(0.5f,-0.5f,0));
//    isx.bitangent =CalculateBiTan(glm::vec3(-0.5f, -0.5f, 0), glm::vec3(-0.5f,0.5f,0), glm::vec3(0.5f,-0.5f,0));

    //TODO: BITANGENT AND TANGENT and T
    return isx;
}


Intersection SquarePlane::GetIntersection(Ray r)
{
    //Transform the ray
    Ray r_loc = r.GetTransformedCopy(transform.invT());
    Intersection result;

    //Ray-plane intersection
    float t = glm::dot(glm::vec3(0,0,1), (glm::vec3(0.5f, 0.5f, 0) - r_loc.origin)) / glm::dot(glm::vec3(0,0,1), r_loc.direction);
    glm::vec4 P = glm::vec4(t * r_loc.direction + r_loc.origin, 1);
    //Check that P is within the bounds of the square
    if(t > 0 && P.x >= -0.5f && P.x <= 0.5f && P.y >= -0.5f && P.y <= 0.5f)
    {
        result.point = glm::vec3(transform.T() * P);
        result.normal = glm::normalize(glm::vec3(transform.invTransT() * glm::vec4(ComputeNormal(glm::vec3(P)), 0)));
        result.object_hit = this;
        result.t = glm::distance(result.point, r.origin);
        result.texture_color = Material::GetImageColorInterp(GetUVCoordinates(glm::vec3(P)), material->texture);
        //TODO: Store the tangent and bitangent
        glm::vec4 obj_tan = glm::vec4(CalculateTangent(glm::vec3(-0.5f, -0.5f, 0), glm::vec3(-0.5f,0.5f,0), glm::vec3(0.5f,-0.5f,0)),0);
        glm::vec4 obj_bit = glm::vec4(CalculateBiTan(glm::vec3(-0.5f, -0.5f, 0), glm::vec3(-0.5f,0.5f,0), glm::vec3(0.5f,-0.5f,0)),0);

        result.tangent =glm::normalize(glm::vec3( transform.T() *obj_tan));
        result.bitangent =glm::normalize(glm::vec3(transform.T()*obj_bit));
        return result;
    }
    return result;
}


glm::vec2 SquarePlane::GetUVCoordinates(const glm::vec3 &point)
{
    return glm::vec2(point.x + 0.5f, point.y + 0.5f);
}

glm::vec3 SquarePlane::ComputeNormal(const glm::vec3 &P)
{
        return glm::vec3(0,0,1);
}



void SquarePlane::create()
{
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
