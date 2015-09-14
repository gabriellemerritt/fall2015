#include <raytracing/ray.h>

Ray::Ray(const glm::vec3 &o, const glm::vec3 &d):
    origin(o),
    direction(glm::normalize(d)),
    transmitted_color(1,1,1)
{}

Ray::Ray(const glm::vec4 &o, const glm::vec4 &d):
    Ray(glm::vec3(o), glm::vec3(d))
{}

Ray::Ray(const Ray &r):
    Ray(r.origin, r.direction)
{
    transmitted_color = r.transmitted_color;
}

Ray Ray::GetTransformedCopy(const glm::mat4 &T) const
{
    glm::vec4 transformedOrigin = T*glm::vec4(this->origin,1 );
    glm::vec4 transformedDirection =T* glm::vec4(this->direction, 0);
    Ray copy (*this);
    copy.direction = glm::vec3(transformedDirection.x,transformedDirection.y,transformedDirection.z);
    copy.origin = glm::vec3(transformedOrigin.x,transformedOrigin.y, transformedOrigin.z);
    return copy;

}
