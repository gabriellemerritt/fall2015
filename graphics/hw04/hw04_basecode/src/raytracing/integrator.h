#pragma once
#include <la.h>
#include <raytracing/ray.h>
#include <raytracing/intersection.h>
#include <raytracing/intersectionengine.h>
#include <scene/scene.h>

class Scene;

//The Integrator class recursively evaluates the path a ray takes throughout a scene
//and computes the color a ray becomes as it bounces.
//It samples the materials, probability density functions, and BRDFs of the surfaces the ray hits
//to do this.

class Integrator
{
public:
    Integrator();
    Integrator(Scene *s);
    glm::vec3 TraceRay(Ray r, unsigned int depth);
    void SetDepth(unsigned int depth);

    Scene* scene;
    IntersectionEngine* intersection_engine;
    glm::vec3 DirectTraceRay(Ray r, unsigned int depth);
    glm::vec3 DirectLight (Ray &r, const Intersection &isx, const glm::vec3 &isx_point,Geometry* light );


protected:
    unsigned int max_depth;//Default value is 5.
};

//class DirectLightingIntegrator: Integrator
//{
//public:
//    DirectLightingI
//    glm::vec3 DirectTraceRay(Ray r, unsigned int depth);
//};
