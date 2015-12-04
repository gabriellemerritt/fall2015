#include <raytracing/integrator.h>


Integrator::Integrator():
    max_depth(5)
{
    scene = NULL;
    intersection_engine = NULL;
}

//Basic ray trace
glm::vec3 Integrator::TraceRay(Ray r, unsigned int depth)
{
       // check if hit depth limit
        if( depth <= 0)
        {
            return glm::vec3(0,0,0);
        }
        // get intersection from intersection engine
        Intersection cur_obj = intersection_engine->GetIntersection(r);
        // testing if I hit anything
        if (cur_obj.object_hit == NULL)
        {
            return glm::vec3(0);
        }
        // presetting returned color to black
        glm::vec3 combinedColor = glm::vec3(0);
        glm::vec3 pixelColor = glm::vec3(0,0,0);
        // get rid of shadow acne
        glm::vec3 isxp = cur_obj.point + cur_obj.normal*0.001f;
       // check if transparent
        bool transparent = false;
         // check if transparent to see if its indices of refraction are non zero
        if((cur_obj.object_hit->material->refract_idx_in)>0 || (cur_obj.object_hit->material->refract_idx_out > 0))
        {
                //going in
                float eta =0.0f;
               // going out of refractive material
                if(glm::dot(r.direction, cur_obj.normal) > 0 )
                {
                    eta = (cur_obj.object_hit->material->refract_idx_in) / (cur_obj.object_hit->material->refract_idx_out);
                }
                // going into material
                else if ( glm::dot(r.direction,cur_obj.normal) < 0 )
                {
                   eta = cur_obj.object_hit->material->refract_idx_out / cur_obj.object_hit->material->refract_idx_in;
                }
                // find ray direction
                glm::vec3 refrac_direction = glm::refract(r.direction, cur_obj.normal, eta);
                // create ray
                Ray transRay = Ray(isxp, refrac_direction);
                //recurse to find color
                combinedColor = TraceRay(transRay,(depth-1));
                transparent = true;

        }
        // if specular surface
        if  (cur_obj.object_hit->material->reflectivity > 0)
        {
            //find direction of reflection
          glm::vec3 reflection_direction =   glm::reflect(r.direction, cur_obj.normal);
          Ray reflection =Ray(isxp,reflection_direction);
          // recursion
               combinedColor += TraceRay(reflection,(depth -1));
        }

        // Loop through light sources to add intensity to points hit by light
           for (  Geometry *light: scene->lights)
           {
               glm::vec3 light_pos = light->transform.position();
               glm::vec3 light_dir = light->transform.position() - isxp;
               // create a ray from intersection point to light source
               Ray incoming_ray = Ray(isxp,light_dir);
               Intersection lightisx = intersection_engine->GetIntersection(incoming_ray);
               if  (cur_obj.object_hit->material->reflectivity > 0)
              {
                   // fequal wasnt working so this is  a bad hack to see if diffuse
               }
              else
               {
                   combinedColor += cur_obj.object_hit->material->EvaluateReflectedEnergy(cur_obj, r.direction,incoming_ray.direction)*
                                        cur_obj.intersectColor;
               }
               //if ray from intersection to light source hit s a light or transparent object increase intensity (for some reason this works it seems weird)

               if (lightisx.object_hit == light||transparent )
               {
                      combinedColor += cur_obj.object_hit->material->EvaluateReflectedEnergy(cur_obj,-r.direction,-incoming_ray.direction);
               }


           }
      return combinedColor;

}

void Integrator::SetDepth(unsigned int depth)
{
    max_depth = depth;
}
//bool  ShadowTest(const glm::vec3 &point);
//{
//    return false;
//}
