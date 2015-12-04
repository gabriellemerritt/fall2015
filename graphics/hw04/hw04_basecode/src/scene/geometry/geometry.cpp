#include <scene/geometry/geometry.h>
glm::vec3 ComponentMult3(const glm::vec3 &a, const glm::vec3 &b);
glm::vec3 ComponentMult3(const glm::vec3 &a, const glm::vec3 &b)
{
    return glm::vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

float Geometry::RayPDF(const Intersection &isx, const Ray &ray)
{
    //TODO
    //The isx passed in was tested ONLY against us (no other scene objects), so we test if NULL
    //rather than if != this.
    if(isx.object_hit == NULL)
    {
        return 0;
    }
//       ComputeArea();
        float pdfRadius =  glm::distance2(ray.origin, isx.point);
        //find surface normal at point
        glm::vec3 ray_dir = -glm::normalize(ray.direction);
//        glm::vec3 peek =   glm::length(isx.normal);
        float cosTheta =(glm::dot(isx.normal, ray_dir));

        float pdf = pdfRadius/(cosTheta*area);
        return pdf;
    //Add more here
}

glm::vec3 Geometry::CalculateTangent(const glm::vec3 &point1, const glm::vec3 &point2, const glm::vec3 &point3)
{
    glm::vec2 uv1 = GetUVCoordinates(point1);
    glm::vec2 uv2 = GetUVCoordinates(point2);
    glm::vec2 uv3 = GetUVCoordinates(point3);

    glm::vec3 dPoint1 = point2 - point1;
    glm::vec3 dPoint2 = point3 - point1;
    glm::vec2 duv1 = uv2 - uv1;
    glm::vec2 duv2 = uv3 - uv1;

    glm::vec3 tangent = (duv2.y*dPoint1 - duv1.y*dPoint2)/(duv2.y*duv1.x - duv1.y*duv2.x);
    return tangent;
}

glm::vec3 Geometry::CalculateBiTan( const glm::vec3 &point1, const glm::vec3 &point2, const glm::vec3 &point3)
{
    glm::vec3 tangent = CalculateTangent(point1, point2, point3);

    glm::vec2 uv1 =GetUVCoordinates(point1);
    glm::vec2 uv2 = GetUVCoordinates(point2);
//    glm::vec2 uv3 = GetUVCoordinates(point3);

    glm::vec3 dPoint1 = point2 - point1;
    glm::vec2 duv1 = uv2 - uv1;
    glm::vec3 bitangent = (dPoint1 - (duv1.x*tangent)) / duv1.y;

    return bitangent;
}
