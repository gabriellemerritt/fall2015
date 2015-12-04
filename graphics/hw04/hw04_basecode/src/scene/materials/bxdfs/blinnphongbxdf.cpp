#include <scene/materials/bxdfs/blinnphongbxdf.h>

glm::vec3 blinnPhongBxDF::EvaluateScatteredEnergy(const glm::vec3 &wo, const glm::vec3 &wi) const
{
    glm::vec3 energy = diffuse_color/PI;
     return energy;
}
glm::vec3 blinnPhongBxDF::EvaluateHemisphereScatteredEnergy(const glm::vec3 &wo, int num_samples, const glm::vec2 *samples) const
{
    //TODO
    return glm::vec3(0);
}
