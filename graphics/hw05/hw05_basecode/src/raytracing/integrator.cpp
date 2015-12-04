#include <raytracing/integrator.h>
#include <time.h> 
#define SAMPLE_SIZE 10

Ray randomWi(Geometry *light_source,  glm::vec3 isx_point);
float powerHeuristic(float nf, float ng, float fPdf, float gPdf)
{
   float f = nf*fPdf;
   float g = ng*gPdf;
    return ((f*f)/ (f*f + g*g));
}

Integrator::Integrator():
    max_depth(5)
{
    scene = NULL;
    intersection_engine = NULL;
}

Integrator::Integrator(Scene *s):
    max_depth(5)
{
    scene = s;
    intersection_engine = NULL;
}

Ray randomWi(Geometry* light_source,  glm::vec3 isx_point)
{
    std::mt19937 rand_factor(rand());
    std::uniform_real_distribution<float> uni_dis(0.0f,1.0f);
    float randomR = uni_dis(rand_factor);
    float randomT  = uni_dis(rand_factor);
    Intersection light_isx =light_source->RandomSample(randomR, randomT);
    glm::vec3 dir_vec = light_isx.point - isx_point;
    Ray sampleRay = Ray(isx_point, dir_vec);
    return sampleRay;
}

glm::vec3 ComponentMult(const glm::vec3 &a, const glm::vec3 &b)
{
    return glm::vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

void Integrator::SetDepth(unsigned int depth)
{
    max_depth = depth;
}

glm::vec3 Integrator::TraceRay(Ray r, unsigned int depth)
{
    glm::vec3 dir_lighting =DirectTraceRay(r,depth);
    return dir_lighting;
}

glm::vec3 Integrator::DirectTraceRay(Ray r, unsigned int depth)
{
    glm::vec3 color = glm::vec3(0,0,0);
    if(depth > max_depth)
    {
        return glm::vec3(0,0,0);
    }

    // shoot ray
    // find intersection
    Intersection isx  = intersection_engine->GetIntersection(r);
    if( isx.object_hit == NULL)
    {
        return glm::vec3(0);
    }

    glm::vec3 isx_point = isx.point + isx.normal*0.001f;
    glm::vec3 LD = glm::vec3(0);
    if(isx.object_hit->material->is_light_source)
    {
        color= ComponentMult(isx.object_hit->material->base_color,isx.texture_color);
        return color;
    }
    for (Geometry *light: scene->lights)
    {
        LD += DirectLight(r,isx,isx_point, light); 
        float ss = 1.0f/SAMPLE_SIZE;
        color = color+ LD*ss;
    }
    return color;
}
//glm::vec3 Integrator::DirectLight (Ray &r, const Intersection &isx, const glm::vec3 &isx_point, Geometry* light )
//{
//    glm::vec3 LD;
//    for(int i = 0; i < SAMPLE_SIZE; i++)
//    {

//        Ray sample = randomWi(light, isx_point);
//        Intersection light_isx = intersection_engine->GetIntersection(sample);
//        if(light_isx.object_hit != light)
//        {
//            LD+= glm::vec3(0);
//        }
//        else
//        {
//            float pdf;
//            float light_pdf = light->RayPDF(light_isx, sample);
//            glm::vec3 f= isx.object_hit->material->EvaluateScatteredEnergy(isx, sample.direction, r.direction,pdf);
//            float absdot = glm::abs(glm::dot(sample.direction, isx.normal))/ light_pdf; // intersection with light
//            glm::vec3 Ld =light->material->EvaluateScatteredEnergy(light_isx,-sample.direction, -sample.direction,pdf);
//            glm::vec3 l  = ComponentMult(f,Ld);
//             LD += l*absdot; // This stays at 1
//             // FOR power heuristic
//             // LD += w_light *l*absdot + w_brdf* isx.object_hit->material.brdf_sampleandevaluatescattered
//        }
//    }
//    return LD;
//}
glm::vec3 Integrator::DirectLight (Ray &r, const Intersection &isx, const glm::vec3 &isx_point, Geometry* light )
{
    float w_light;
    float w_bxdf;
    glm::vec3 LD;
    for(int i = 0; i < SAMPLE_SIZE; i++)
    {

        Ray sample = randomWi(light, isx_point);
        Intersection light_isx = intersection_engine->GetIntersection(sample);
        if(light_isx.object_hit == NULL)
        {
               LD+=glm::vec3(0);
         }
        else if(light_isx.object_hit != light)
        {
            LD+= glm::vec3(0);
        }
        else
        {
            glm::vec3 wi_sample;
            float light_pdf = light->RayPDF(light_isx, sample);
            float light_bxdf_pdf;

            glm::vec3 f= isx.object_hit->material->EvaluateScatteredEnergy(isx, r.direction,
                                                                           glm::normalize(sample.direction),light_bxdf_pdf);

            float absdot = glm::abs(glm::dot(sample.direction, isx.normal))/ light_pdf; // intersection with light
            glm::vec3 Ld =light->material->EvaluateScatteredEnergy(light_isx,
                                      -r.direction,sample.direction,light_bxdf_pdf);

            glm::vec3 l  = ComponentMult(f,Ld);
//             LD += l*absdot; // This stays at 1
             // BRDF power heuristic
             w_light  = powerHeuristic(1,1,light_pdf, light_bxdf_pdf);
             // check this
             glm::vec3 bxdf = CalculateBxDF(isx, w_bxdf, wi_sample, r);

             // LD += w_light *l*absdot + w_brdf* isx.object_hit->material.brdf_sampleandevaluatescattered
              LD+= w_light*l*absdot + w_bxdf*bxdf;
        }
    }
    return cmpMulti(LD,isx.object_hit->material->base_color);
}
glm::vec3 Integrator::CalculateBxDF(const Intersection &isx, float &w_bxdf_ret,  glm::vec3 &wiW_ret, const Ray &r )
{
    float bxdf_pdf_ret;
    float bxdf_light_pdf = 0;
   glm::vec3 energy =  isx.object_hit->material->SampleAndEvaluateScatteredEnergy(isx, -r.direction, wiW_ret, bxdf_pdf_ret);
   Ray wiW = Ray(isx.point, wiW_ret);
   Intersection reflect_isx = intersection_engine->GetIntersection(wiW);
   if(reflect_isx.object_hit == NULL)
   {
       w_bxdf_ret = 0;
       return energy;
   }
  else if(reflect_isx.object_hit->material->is_light_source)
   {
       bxdf_light_pdf = reflect_isx.object_hit->RayPDF(reflect_isx,wiW);
   }
   w_bxdf_ret = powerHeuristic(1,1,bxdf_pdf_ret, bxdf_light_pdf);
    return energy;

}

//glm::vec3 Integrator::EstimateLightIndirect(const Intersection &light_isx )
//{


//}
//glm::vec3 Integrator::EstimateBxDFIndirect()
//{

//}
