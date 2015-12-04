#include <scene/materials/bxdfs/specularreflectionBxDF.h>

float SpecularReflectionBxDF::PDF(const glm::vec3 &wo, const glm::vec3 &wi) const
{
    float valid_reflect = glm::length(glm::normalize(wo+wi) - glm::vec3(0,0,1));
   if(valid_reflect <= 0.000001f )
    {
        return 1.0f;
    }

    return 0.0f;
}
glm::vec3 SpecularReflectionBxDF::EvaluateScatteredEnergy(const glm::vec3 &wo, const glm::vec3 &wi, float &pdf_ret) const
{
    // THIS SEEMS VERY WRONG
    float valid_reflect =glm::length( glm::normalize(wo+wi) - glm::vec3(0,0,1));
    if(valid_reflect <= 0.000001f )
    {
        pdf_ret = 1.0f;
        return reflection_color;
    }
     pdf_ret = PDF(wo,wi);
    return glm::vec3(0);
}
glm::vec3 SpecularReflectionBxDF::EvaluateHemisphereScatteredEnergy(const glm::vec3 &wo, int num_samples, const glm::vec2 *samples) const
{
    //TODO
    return glm::vec3(0);
}

glm::vec3 SpecularReflectionBxDF::SampleAndEvaluateScatteredEnergy(const glm::vec3 &wo, glm::vec3 &wi_ret, float rand1, float rand2, float &pdf_ret) const
{
    //TODO
    wi_ret =  glm::vec3( -wo.x, -wo.y, wo.z);
    glm::vec3 energy = EvaluateScatteredEnergy(wo,wi_ret,pdf_ret);
    pdf_ret = PDF(wo,wi_ret );
    return energy;
}
