#include <raytracing/intersection.h>
bool withinViewingFrustrum(const float t, const float near_clip, const float far_clip);

Intersection::Intersection():
    point(glm::vec3(0)),
    normal(glm::vec3(0)),
    t(-1),
   intersectColor(glm::vec3(1,1,1))
{
    object_hit = NULL;
}

IntersectionEngine::IntersectionEngine()
{
    scene = NULL;
}

Intersection IntersectionEngine:: GetIntersection(Ray r)
{
    //
    Geometry*  closeGeom = NULL;
     for (  Geometry *g: scene->objects)
     {

         if (closeGeom == NULL)
         {
             closeGeom = g;
         }
         if (g->GetIntersection(r).object_hit != NULL)
          {
             if(( g->GetIntersection(r).t < closeGeom->GetIntersection(r).t) ||
                     fequal(closeGeom->GetIntersection(r).t, -1.0f))
             {
                 //this only works when the ray's origin is the camera
                 //try doing camera.viewmatrix * intersection.point and checking the Z coord of the new point in camera space
                  glm::vec3 view = glm::vec3( scene->camera.ViewMatrix()*glm::vec4(g->GetIntersection(r).point,1));
                  glm::vec3 close_view =  glm::vec3( scene->camera.ViewMatrix()*glm::vec4(closeGeom->GetIntersection(r).point,1));
                  if(glm::abs(view.z) < glm::abs(close_view.z))
                  {
                      closeGeom = g;
                  }
             }
         }
     }
    return closeGeom->GetIntersection(r);
}
bool withinViewingFrustrum(const float t, const float near_clip, const float far_clip)
{
    if((t >near_clip) && t < far_clip)
    {
        return true;
    }
    return false;
}
