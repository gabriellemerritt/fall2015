#include <raytracing/integrator.h>
#include <time.h> 
#define SAMPLE_SIZE 10000

Ray randomWi(Geometry *light_source,  glm::vec3 isx_point);
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
    glm::vec3 color = glm::vec3(.2,.2,.2); // ambient light
    if(depth > max_depth)
    {
        return glm::vec3(.2,.2,.2);
    }

    // shoot ray
    // find intersection
    Intersection isx  = intersection_engine->GetIntersection(r);
    if( isx.object_hit == NULL)
    {
       return (isx.texture_color + glm::vec3(.2,.2,.2));
//        return glm::vec3(.2,.2,.2);
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
        LD+= DirectLight(r,isx, isx_point,light);
//        for(int i = 0; i < SAMPLE_SIZE; i++)
//        {
//            Ray sample = randomWi(light, isx_point);
//            Intersection light_isx = intersection_engine->GetIntersection(sample);
//            if(light_isx.object_hit != light)
//            {
//                LD+= glm::vec3(0);
//            }
//            else
//            {
//                float pdf = light->RayPDF(light_isx, sample);

//                glm::vec3 f= isx.object_hit->material->EvaluateScatteredEnergy(isx, sample.direction, r.direction);
//                float absdot = glm::abs(glm::dot(sample.direction, isx.normal))/ pdf; // intersection with light
//                glm::vec3 Ld =light->material->EvaluateScatteredEnergy(light_isx,-sample.direction, -sample.direction);
//                glm::vec3 l  = ComponentMult(f,Ld);
//    //            glm::vec3 direct_light = ComponentMult(sample.transmitted_color,l);
//                 LD += l*absdot; // This stays at 1
//            }
//        }
        float ss = 1.0f/SAMPLE_SIZE;
        color = color+ LD*ss;
    }


    return color;

}

glm::vec3 Integrator::DirectLight (Ray &r, const Intersection &isx, const glm::vec3 &isx_point, Geometry* light )
{
    glm::vec3 LD;
    for(int i = 0; i < SAMPLE_SIZE; i++)
    {

        Ray sample = randomWi(light, isx_point);
        Intersection light_isx = intersection_engine->GetIntersection(sample);
        if(light_isx.object_hit != light)
        {
            LD+= glm::vec3(0);
        }
        else
        {
            float light_pdf = light->RayPDF(light_isx, sample);
            glm::vec3 f= isx.object_hit->material->EvaluateScatteredEnergy(isx, sample.direction, r.direction);
            float absdot = glm::abs(glm::dot(sample.direction, isx.normal))/ light_pdf; // intersection with light
            glm::vec3 Ld =light->material->EvaluateScatteredEnergy(light_isx,-sample.direction, -sample.direction);
            glm::vec3 l  = ComponentMult(f,Ld);
             LD += l*absdot; // This stays at 1
             // FOR power heuristic
             // LD += w_light *l*absdot + w_brdf* isx.object_hit->material.brdf_sampleandevaluatescattered
        }
    }
    return LD;
}
