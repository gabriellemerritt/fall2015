#include <scene/materials/weightedmaterial.h>

WeightedMaterial::WeightedMaterial() : Material(){}
WeightedMaterial::WeightedMaterial(const glm::vec3 &color) : Material(color){}

glm::vec3 WeightedMaterial::EvaluateScatteredEnergy(const Intersection &isx, const glm::vec3 &woW, const glm::vec3 &wiW, float &pdf_ret, BxDFType flags) const
{
    //TODO
    // put incoming and outgoing ray directions in object space!!!!!!!!!!!
    std::mt19937 rand_factor(rand());
    std::uniform_real_distribution<float> uni_dis(0.0f,1.0f);
    glm::vec3 woO;
    glm::vec3 wiO;
    glm::mat4 object_space_transformation;
    object_space_transformation[0] = glm::vec4(isx.tangent.x, isx.bitangent.x,isx.normal.x,0);
    object_space_transformation[1] = glm::vec4(isx.tangent.y, isx.bitangent.y,isx.normal.y,0);
    object_space_transformation[2] = glm::vec4(isx.tangent.z, isx.bitangent.z,isx.normal.z,0);
    object_space_transformation[3] = glm::vec4(0,0,0,1);

    wiO = glm::vec3(object_space_transformation*glm::vec4(wiW,0));
    woO = glm::vec3(object_space_transformation*glm::vec4(woW,0));
    float rand_sample = uni_dis(rand_factor);

    // maybe try random number generator and pick bxdf that it falls in
    int bin_num = this->findBin(rand_sample);

//    glm::vec3 tex_col = isx.texture_color;
//    glm::vec3 mat_color = cmpMulti(tex_col, this->base_color);

    glm::vec3 energy;
//    for (i = 0; i <  bxdf_weights.size(); i++)
//    {
//        energy += bxdf_weights.at(i)*bxdfs.at(i)->EvaluateScatteredEnergy(woO,wiO);
//    }
    energy = bxdf_weights.at(bin_num)*bxdfs.at(bin_num)->EvaluateScatteredEnergy(woO,wiO,pdf_ret);
    return energy;
}
int WeightedMaterial::findBin ( float rand_sample) const
{
    float bin_weight = 0;
    for (int i =0 ;  i < bxdf_weights.size(); i++)
    {
        bin_weight +=bxdf_weights.at(i);
       if( rand_sample <= bin_weight)
       {
           return i;
       }
    }
}

glm::vec3 WeightedMaterial::SampleAndEvaluateScatteredEnergy(const Intersection &isx, const glm::vec3 &woW, glm::vec3 &wiW_ret, float &pdf_ret, BxDFType flags) const
{
    std::mt19937 rand_factor(rand());
    std::uniform_real_distribution<float> uni_dis(0.0f,1.0f);
    glm::vec3 woO;
//    glm::vec3 wiO;
    glm::mat4 object_space_transformation;
    object_space_transformation[0] = glm::vec4(isx.tangent.x, isx.bitangent.x,isx.normal.x,0);
    object_space_transformation[1] = glm::vec4(isx.tangent.y, isx.bitangent.y,isx.normal.y,0);
    object_space_transformation[2] = glm::vec4(isx.tangent.z, isx.bitangent.z,isx.normal.z,0);
    object_space_transformation[3] = glm::vec4(0,0,0,1);

    woO = glm::vec3(object_space_transformation*glm::vec4(woW,0));
    float rand_sample = uni_dis(rand_factor);
    float rand_sample2 = uni_dis(rand_factor);
    float rand_sample3 = uni_dis(rand_factor);


    int bin_num = findBin(rand_sample);
    glm::vec3 wiO_ret;
    glm::vec3 energy =  bxdfs.at(bin_num)->SampleAndEvaluateScatteredEnergy(woO,wiO_ret,rand_sample2, rand_sample3, pdf_ret);

    glm::mat4 world_space_trans;
    world_space_trans[0] = glm::vec4(isx.tangent,0);
    world_space_trans[1] = glm::vec4(isx.bitangent,0);
    world_space_trans[2] = glm::vec4(isx.normal,0);
    world_space_trans[3] = glm::vec4(0,0,0,1);

    wiW_ret = glm::vec3(world_space_trans*glm::vec4(wiO_ret,0));


    return energy;

    // call brdf sample and evaluate
    // maybe try random number generator and pick bxdf that it falls in
    // transform output into world frame


    return energy;
}
