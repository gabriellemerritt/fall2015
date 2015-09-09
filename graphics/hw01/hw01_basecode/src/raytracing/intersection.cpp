#include <raytracing/intersection.h>

Intersection::Intersection():
    point(glm::vec3(0)),
    normal(glm::vec3(0)),
    t(-1)
{
    object_hit = NULL;
}

IntersectionEngine::IntersectionEngine()
{
    scene = NULL;
}

Intersection IntersectionEngine::GetIntersection(Ray r)
{
    //TODO
    return Intersection();
}
