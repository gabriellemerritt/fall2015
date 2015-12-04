#include <scene/materials/lambertmaterial.h>

LambertMaterial::LambertMaterial():Material()
{}

LambertMaterial::LambertMaterial(const glm::vec3 &color):Material(color)
{

}

glm::vec3 LambertMaterial::EvaluateReflectedEnergy(const Intersection &isx,
                                                   const glm::vec3 &outgoing_ray, const glm::vec3 &incoming_ray)
{


      float lambert_shading = glm::clamp( glm::dot( isx.normal, incoming_ray),0.0f,1.0f);
      glm::vec3 surfaceColor = isx.intersectColor;
      if(reflectivity)
      {
         surfaceColor =(1 -reflectivity)*surfaceColor;
      }
      glm::vec3 lambert = surfaceColor*lambert_shading;


    return lambert;
}
