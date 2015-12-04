#include "bvhnode.h"
//float compareNearSwap(float t_small, float t_compare)
//{
//    if(t_small > t_compare)
//    {
//        return t_small;
//    }
//    return t_compare;
//}

//float compareFarSwap(float t_1, float t_far)
//{
//    if(t_1 < t_far)
//    {
//        return t_1;
//    }
//    return t_far;
//}

BVHNode::BVHNode()
{
   BVHNode(glm::vec3(5.0f,5.0f,5.0f), glm::vec3(-5.0f, -5.0f, -5.0f));
   this->leftChild = NULL;
   this->rightChild = NULL;
}

BVHNode::BVHNode(const glm::vec3 &max,  const glm::vec3 &min )
{
    bb = new BoundingBox(max, min);
    bb->create();
    leftChild = NULL;
    rightChild = NULL;

}
/*Intersection BVHNode::GetIntersection(Ray r)
{
    glm::vec3 min = this->bb->min_bounds;
    glm::vec3 max = this->bb->max_bounds;
    if ( this->leftChild == NULL && this->rightChild == NULL )
    {
        return this->geo->GetIntersection(r);
    }

    float t_0, t_1, t_near, t_far;
//    static_assert(std::numeric_limits<float>::is_iec559, "IEEE 754 required");
    t_near = -INFINITY;
    t_far = INFINITY;

    //calculate t0, t1 for all planes
    for (int i = 0; i < 3; i++)
    {
        // weird
        if(fequal(r.direction[i],0.0f));
        {
           if(r.origin[i] < this->bb->min_bounds[i]  || r.origin[i] >this->bb->max_bounds[i])
           {
              isx.object_hit= NULL;
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

        t_near = compareNearSwap(t_0,t_near);
        t_far  = compareFarSwap(t_1, t_far);
    }
    if(t_near > t_far || t_near < 0 || t_far < 0)
    {
        isx.object_hit = NULL;
        isx.t = -1.0f;
        return isx;
    }
   Intersection i1  = this->leftChild->GetIntersection(r);
   Intersection i2 = this->rightChild->GetIntersection(r);
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

}*/

