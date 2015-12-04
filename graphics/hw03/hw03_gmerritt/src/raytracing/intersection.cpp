#include <raytracing/intersection.h>
bool withinViewingFrustrum(const float t, const float near_clip, const float far_clip);
float compareNearSwap(float t_small, float t_compare);
float compareFarSwap(float t_1, float t_far);
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
    this->rootNode = NULL;
}

float icompareNearSwap(float t_small, float t_compare)
{
    if(t_small > t_compare)
    {
        return t_small;
    }
    return t_compare;
}

float icompareFarSwap(float t_1, float t_far)
{
    if(t_1 < t_far)
    {
        return t_1;
    }
    return t_far;
}
Intersection IntersectionEngine::BbIsx(Ray r, BVHNode* root)
{

    bool hit_face = true;
    glm::vec3 min = root->bb->min_bounds;
    glm::vec3 max = root->bb->max_bounds;
    if ( root->leftChild == NULL && root->rightChild == NULL )
    {
        return root->geo->GetIntersection(r);
    }

    float t_0, t_1, t_near, t_far;
    t_near = -INFINITY;
    t_far = INFINITY;

    //calculate t0, t1 for all planes
    for (int i = 0; i < 3; i++)
    {
        if(fequal(r.direction[i],0.0f))
        {
           if(r.origin[i] < root->bb->min_bounds[i]  || r.origin[i] >root->bb->max_bounds[i])
           {
               continue;
           }
        }
        t_0 = (min[i]- r.origin[i])/ r.direction[i];
        t_1 = (max[i] - r.origin[i])/ r.direction[i];

        if(t_0 > t_1)
        {
            float swap = t_0;
            t_0 = t_1;
            t_1 = swap;

        }

        t_near = glm::max(t_0,t_near);
        t_far  = glm::min(t_1, t_far);
    }


    if(t_near > t_far || t_near < 0 || t_far < 0)
    {
        Intersection isx;
        isx.object_hit = NULL;
        isx.t = -1.0f;
        return isx;
    }

   Intersection i1  = BbIsx(r, root->leftChild);
   Intersection i2 = BbIsx(r, root->rightChild);
   if ( i1.object_hit == NULL)
   {
       return i2;
   }
   if(i1.object_hit != NULL && i2.object_hit != NULL)
   {
       if (i2.t < i1.t)
       {
           return i2;
       }
   }
   return i1;

}

Intersection IntersectionEngine::GetIntersection(Ray r)
{
    Intersection isx =BbIsx(r,rootNode);
//    if(isx.object_hit != NULL)
//    {
//        qDebug() << "YEAH";
//    }
    return isx;
}

//Intersection IntersectionEngine:: GetIntersection(Ray r)
//{
//    //  recursively go down tree with intersections (left and right intersections) and retunr the intersection with smallest t value
//    Geometry*  closeGeom = NULL;
//     for (  Geometry *g: scene->objects)
//     {

//         if (closeGeom == NULL)
//         {
//             closeGeom = g;
//         }
//         if (g->GetIntersection(r).object_hit != NULL)
//          {
//             if(( g->GetIntersection(r).t < closeGeom->GetIntersection(r).t) ||
//                     fequal(closeGeom->GetIntersection(r).t, -1.0f))
//             {
//                 //this only works when the ray's origin is the camera
//                 //try doing camera.viewmatrix * intersection.point and checking the Z coord of the new point in camera space
//                  glm::vec3 view = glm::vec3( scene->camera.ViewMatrix()*glm::vec4(g->GetIntersection(r).point,1));
//                  glm::vec3 close_view =  glm::vec3( scene->camera.ViewMatrix()*glm::vec4(closeGeom->GetIntersection(r).point,1));
//                  if(glm::abs(view.z) < glm::abs(close_view.z))
//                  {
//                      closeGeom = g;
//                  }
//             }
//         }
//     }
//    return closeGeom->GetIntersection(r);
//}
bool withinViewingFrustrum(const float t, const float near_clip, const float far_clip)
{
    if((t >near_clip) && t < far_clip)
    {
        return true;
    }
    return false;
}
