#ifndef BVHNODE_H
#define BVHNODE_H
#include <scene/geometry/boundingbox.h>
#include <scene/geometry/geometry.h>
class Geometry;
class BVHNode
{
public:
    BVHNode();
    BVHNode(const glm::vec3 &max, const glm::vec3 &min);
    BVHNode* leftChild;
    BVHNode* rightChild;
    BoundingBox*  bb;
    QString name;
//    Intersection GetIntersection(Ray r);
//    Intersection isx;
    Geometry* geo;
private:


};

#endif // BVHNODE_H
