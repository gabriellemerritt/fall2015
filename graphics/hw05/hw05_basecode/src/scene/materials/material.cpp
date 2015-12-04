#include <scene/materials/material.h>
#include <QColor>
#include <math.h>
#include <random>


glm::vec3 ComponentMult2(const glm::vec3 &a, const glm::vec3 &b);
glm::vec3 ComponentMult2(const glm::vec3 &a, const glm::vec3 &b)
{
    return glm::vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

Material::Material() :
    Material(glm::vec3(0.5f, 0.5f, 0.5f))
{}

Material::Material(const glm::vec3 &color):
    name("MATERIAL"),
    bxdfs(),
    is_light_source(false),
    base_color(color),
    intensity(0)
{
    texture = NULL;
    normal_map = NULL;
}




glm::vec3 Material::EvaluateScatteredEnergy(const Intersection &isx, const glm::vec3 &woW, const glm::vec3 &wiW, float &pdf_ret, BxDFType flags) const
{
    glm::vec3 woO;
    glm::vec3 wiO;
    glm::mat4 object_space_transformation;
    object_space_transformation[0] = glm::vec4(isx.tangent.x, isx.bitangent.x,isx.normal.x,0);
    object_space_transformation[1] = glm::vec4(isx.tangent.y, isx.bitangent.y,isx.normal.y,0);
    object_space_transformation[2] = glm::vec4(isx.tangent.z, isx.bitangent.z,isx.normal.z,0);
    object_space_transformation[3] = glm::vec4(0,0,0,1);

        wiO = glm::vec3(object_space_transformation*glm::vec4(wiW,0));
        woO = glm::vec3(object_space_transformation*glm::vec4(woW,0));
    //out going ray direction woW   // incoming ray direction world frame wiW
   glm::vec3 tex_col = isx.texture_color;
   // make sure this is component wise multiplication
   glm::vec3 energy1 = ComponentMult2(tex_col, this->base_color);
//   qDebug() << " BxDF list :" << bxdfs.size();
//   if(bxdfs.size() <= 0 )
//   {
//       return energy1;
//   }
   glm::vec3 energy = ComponentMult2(energy1,bxdfs.last()->EvaluateScatteredEnergy(woO,wiO,pdf_ret));
   return energy;
}

glm::vec3 Material::SampleAndEvaluateScatteredEnergy(const Intersection &isx, const glm::vec3 &woW, glm::vec3 &wiW_ret, float &pdf_ret, BxDFType flags) const
{
    //TODO
    std::mt19937 rand_factor(rand());
    std::uniform_real_distribution<float> uni_dis(0,1.0f);
    std::uniform_int_distribution<int> bin_dis(0,bxdfs.size()-1);
    glm::vec3 woO;
    glm::vec3 wiO;
    glm::mat4 object_space_transformation;
    object_space_transformation[0] = glm::vec4(isx.tangent.x, isx.bitangent.x,isx.normal.x,0);
    object_space_transformation[1] = glm::vec4(isx.tangent.y, isx.bitangent.y,isx.normal.y,0);
    object_space_transformation[2] = glm::vec4(isx.tangent.z, isx.bitangent.z,isx.normal.z,0);
    object_space_transformation[3] = glm::vec4(0,0,0,1);

//        wiO = glm::vec3(object_space_transformation*glm::vec4(wiW,0));
        woO = glm::normalize(glm::vec3(object_space_transformation*glm::vec4(woW,0)));
    //out going ray direction woW   // incoming ray direction world frame wiW
   glm::vec3 tex_col = isx.texture_color;
   // make sure this is component wise multiplication
   glm::vec3 energy1 = ComponentMult2(tex_col, this->base_color);

   float rand_sample = bin_dis(rand_factor);
   float rand_sample2 = uni_dis(rand_factor);
   float rand_sample3 = uni_dis(rand_factor);



   glm::vec3 wiO_ret;
   glm::vec3 energy =  bxdfs.at(rand_sample)->SampleAndEvaluateScatteredEnergy(woO,wiO_ret,rand_sample2, rand_sample3, pdf_ret);

   glm::mat4 world_space_trans;
   world_space_trans[0] = glm::vec4(isx.tangent,0);
   world_space_trans[1] = glm::vec4(isx.bitangent,0);
   world_space_trans[2] = glm::vec4(isx.normal,0);
   world_space_trans[3] = glm::vec4(0,0,0,1);

   wiW_ret = glm::vec3(world_space_trans*glm::vec4(wiO_ret,0));
   return energy;


}

glm::vec3 Material::EvaluateHemisphereScatteredEnergy(const Intersection &isx, const glm::vec3 &wo, int num_samples, BxDFType flags) const
{
    //TODO
    return glm::vec3(0);
}


glm::vec3 Material::GetImageColor(const glm::vec2 &uv_coord, const QImage* const& image)
{
    if(image == NULL || uv_coord.x < 0 || uv_coord.y < 0 || uv_coord.x >= 1.0f || uv_coord.y >= 1.0f)
    {
        return glm::vec3(1,1,1);
    }
    else
    {
        int X = glm::min(image->width() * uv_coord.x, image->width() - 1.0f);
        int Y = glm::min(image->height() * (1.0f - uv_coord.y), image->height() - 1.0f);
        QColor color = image->pixel(X, Y);
        return glm::vec3(color.red(), color.green(), color.blue())/255.0f;
    }
}

glm::vec3 Material::GetImageColorInterp(const glm::vec2 &uv_coord, const QImage* const& image)
{
    if(image == NULL || uv_coord.x < 0 || uv_coord.y < 0 || uv_coord.x >= 1.0f || uv_coord.y >= 1.0f)
    {
        return glm::vec3(1,1,1);
    }

//    float X = image->width() * uv_coord.x;
//    float Y = image->height() * (1.0f - uv_coord.y);

//    glm::vec2 floors = glm::vec2(floor(X), floor(Y));
//    glm::vec2 ceils = glm::vec2(ceil(X), ceil(Y));
//    ceils = glm::min(ceils, glm::vec2(image->width()-1, image->height()-1));

//    if(ceils.x>0.0&&ceils.y>0.0&&floors.x>0.0&&floors.y>0.0)
//    {
        //Use bilinear interp.
  else
    {
        float X = image->width() * uv_coord.x;
        float Y = image->height() * (1.0f - uv_coord.y);

        glm::vec2 floors = glm::vec2(floor(X), floor(Y));
        glm::vec2 ceils = glm::vec2(ceil(X), ceil(Y));
        ceils = glm::min(ceils, glm::vec2(image->width()-1, image->height()-1));

        QColor qll = image->pixel(floors.x, floors.y); glm::vec3 ll(qll.red(), qll.green(), qll.blue());
        QColor qlr = image->pixel(ceils.x, floors.y); glm::vec3 lr(qlr.red(), qlr.green(), qlr.blue());
        QColor qul = image->pixel(floors.x, ceils.y); glm::vec3 ul(qul.red(), qul.green(), qul.blue());
        QColor qur = image->pixel(ceils.x, ceils.y); glm::vec3 ur(qur.red(), qur.green(), qur.blue());

            float distX = (X - floors.x);
            glm::vec3 color_low = ll * (1-distX) + lr * distX;
            glm::vec3 color_high = ul * (1-distX) + ur * distX;

            float distY = (Y - floors.y);

            glm::vec3 result = (color_low * (1 - distY) + color_high * distY)/255.0f;

            return result;

        glm::vec2 XY(X,Y);
        //Want floor and ceil of both X and Y
        //Do square interp of <X,Y>

        float dist_ll = glm::distance(XY, floors);
        float dist_lr = glm::distance(XY, glm::vec2(ceils.x, floors.y));
        float dist_ul = glm::distance(XY, glm::vec2(floors.x, ceils.y));
        float dist_ur = glm::distance(XY, ceils);
        float sum = dist_ll + dist_lr + dist_ul + dist_ur;

        float llc = (1 - dist_ll/sum);
        float lrc = (1 - dist_lr/sum);
        float ulc = (1 - dist_ul/sum);
        float urc = (1 - dist_ur/sum);

        float checksum = llc + lrc + ulc + urc;

        glm::vec3 final_color = llc * ll + lrc * lr + ulc * ul + urc * ur;
        return final_color/255.0f;
    }
//    return glm::vec3(1,1,1);
}
