#include <scene/materials/phongmaterial.h>

PhongMaterial::PhongMaterial():
    PhongMaterial(glm::vec3(0.5f, 0.5f, 0.5f))
{}

PhongMaterial::PhongMaterial(const glm::vec3 &color):
    Material(color),
    specular_power(10)
{}

glm::vec3 PhongMaterial::EvaluateReflectedEnergy(const Intersection &isx, const glm::vec3 &outgoing_ray, const glm::vec3 &incoming_ray)
{

    float kd, ks, ka;
    kd =0.3333f;
    ks = 0.3333f;
    ka = 0.3333f;
    float theta = glm::dot( isx.normal, incoming_ray);
    float diffuse =  glm::clamp(theta,0.0f,1.0f);
   glm::vec3  D = diffuse* base_color;
   glm::vec3 H = (outgoing_ray - incoming_ray)* 0.5f;
   float reflection = glm::dot(H,glm::normalize(isx.normal));
   glm::vec3 S = glm::max(glm::pow(reflection,specular_power),0.0f)*base_color;
   glm::vec3 A = glm::vec3(.2f,.2f,.2f);
    return kd*D+ ks*S + ka*A;

}
