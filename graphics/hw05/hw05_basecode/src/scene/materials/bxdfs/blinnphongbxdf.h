#pragma once
#include <scene/materials/bxdfs/bxdf.h>


class blinnPhongBxDF: public BxDF
{
public:
    blinnPhongBxDF(const glm::vec3 &color) : BxDF(BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE)), diffuse_color(color)
    {}

    virtual glm::vec3 EvaluateScatteredEnergy(const glm::vec3 &wo, const glm::vec3 &wi) const;
    virtual glm::vec3 EvaluateHemisphereScatteredEnergy(const glm::vec3 &wo, int num_samples, const glm::vec2 *samples) const;
//Member variables
    glm::vec3 diffuse_color;
};
