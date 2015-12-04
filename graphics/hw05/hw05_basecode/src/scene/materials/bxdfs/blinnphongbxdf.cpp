#include <scene/materials/bxdfs/blinnphongbxdf.h>
//DONT USE
glm::vec3 blinnPhongBxDF::EvaluateScatteredEnergy(const glm::vec3 &wo, const glm::vec3 &wi) const
{
    float n = 10;
    //reflectance factor
    float rho_s = .45;
    float k_l = .55;
    glm::vec3 in_ray = glm::normalize(wi);
    glm::vec3 out_ray = glm::normalize(wo);
    float delta = glm::dot(in_ray,out_ray);
    float s =  rho_s* ((PI+2)/ 2*PI)*powf(delta, n);
    glm::vec3 energy = (k_l*diffuse_color/PI )+ diffuse_color*s ;
    return energy;
}
glm::vec3 blinnPhongBxDF::EvaluateHemisphereScatteredEnergy(const glm::vec3 &wo, int num_samples, const glm::vec2 *samples) const
{
    //TODO
    return glm::vec3(0);
}
glm::vec3 BxDF::SampleAndEvaluateScatteredEnergy(const glm::vec3 &wo, glm::vec3 &wi_ret, float rand1, float rand2, float &pdf_ret) const
{
    //TODO
    wi_ret = glm::vec3(0);
    pdf_ret = 0.0f;
    return glm::vec3(0);
}
