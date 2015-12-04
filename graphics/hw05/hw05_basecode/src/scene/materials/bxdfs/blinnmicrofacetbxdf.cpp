#include <scene/materials/bxdfs/blinnmicrofacetbxdf.h>

float GeomTerm(const glm::vec3 &wo, const glm::vec3 &wi, const glm::vec3 &wh);
float FresnelRef(const glm::vec3 &wo, const glm::vec3 &wi);

//float BlinnMicrofacetBxDF::getPDF(const glm::vec3 & wo, const glm::vec3 &wi) const
//{
//    return  PDF(wo,wi);
//}
float BlinnMicrofacetBxDF::PDF(const glm::vec3 &wo, const glm::vec3 &wi) const
{
    // isx to camera/ ligiht
    glm::vec3 wh = (wo + wi)/2.0f;
     float half_angle_dot = glm::dot(wo,wh);
     if(half_angle_dot <=0.0f)
     {
         return 0.0f;
     }
    float costheta = glm::dot(wo,wi);
    float pdf = exponent+1.0f *powf(costheta, exponent) / (2.0f * PI * 4.0 *glm::dot(wo,wh));
    return pdf;
}
float FresnelRef(const glm::vec3 &wo, const glm::vec3 &wi)
{
    // ASK ADAM  is nt and ni just 1?? !
    float nt = 1000000.0f;
    float ni = 1.0f;
    float cost_i = glm::dot(wi, glm::vec3(0,0,1));
    float cost_o = glm::dot(wo, glm::vec3(0,0,1));
    float r_l =  (nt*cost_i - ni*cost_o)/(nt*cost_i + ni*cost_o);
    float r_p = (ni*cost_i - nt*cost_o)/(ni*cost_i + nt*cost_o);
    float fres_ref = (powf(r_l,2.0f)+powf(r_p,2.0f))/2.0f;
    return fres_ref;
}

float GeomTerm(const glm::vec3 &wo, const glm::vec3 &wi, const glm::vec3 &wh)
{
    glm::vec3 N(0,0,1);
    float Masking =( 2*glm::dot(N,wh)*glm::dot(N,wo))/ glm::dot(wo,wh);
    float Shadow = ( 2*glm::dot(N,wh)*glm::dot(N,wi))/ glm::dot(wo,wh);
    float G = glm::min(1.0f, Shadow);
    G = glm::min(G, Masking );
    return G;
}

glm::vec3 BlinnMicrofacetBxDF::EvaluateScatteredEnergy(const glm::vec3 &wo, const glm::vec3 &wi, float &pdf_ret) const
{
    // WHAT IS N
    float cost_i = glm::dot(wi, glm::vec3(0,0,1));
    float cost_o = glm::dot(wo, glm::vec3(0,0,1));
    glm::vec3 N(0,0,1);
    glm::vec3 wh = (wo + wi)/2.0f;
    float dist = powf(glm::dot(wh,N),exponent)*(exponent +2)/(2.0f*PI);
    float G = GeomTerm(wo,wi,wh);
    float Fr = FresnelRef(wo,wi);
    float bxdf = (dist*G*Fr)/(4*cost_o*cost_i);
    pdf_ret = PDF(wo,wi);

//    return reflection_color*bxdf;
//      glm::vec3 energy =  -wo + wh*2.0f*glm::dot(wo,wh);
     return  bxdf*reflection_color;
}


glm::vec3 BlinnMicrofacetBxDF::SampleAndEvaluateScatteredEnergy(const glm::vec3 &wo, glm::vec3 &wi_ret, float rand1, float rand2, float &pdf_ret) const
{
    //TODO
    float cosTheta = powf(rand1, (1.0f /(exponent+1)));
    float sinTheta = glm::sqrt(glm::max(0.0f, (1.0f- cosTheta*cosTheta)));
    float phi = rand2*2.0f*PI;
    // Z = costheta
    // y = sin(phi)* sintheta
    //x = cos(phi) *sinTheta
    glm::vec3 wh =glm::vec3( glm::cos(phi)*sinTheta, glm::sin(phi)*sinTheta, cosTheta);
    if ( glm::dot(wo, wh) < 0 )
    {
         wh = -1.0f* wh;
     }
    // Randomly generate direction in lobe
    // sphere direction is spherical coordinates pg 697
    wi_ret = -wo + 2.0f *glm::dot(wo,wh) * wh;
    glm::vec3 energy = EvaluateScatteredEnergy(wo, wi_ret,pdf_ret);
    pdf_ret = PDF(wo, wi_ret);
    return energy;
}
glm::vec3 BlinnMicrofacetBxDF::EvaluateHemisphereScatteredEnergy(const glm::vec3 &wo, int num_samples, const glm::vec2 *samples) const
{
    //TODO
    return glm::vec3(0);
}

