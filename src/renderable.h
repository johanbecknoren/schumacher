#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "AABB.h"
#include "intersectionpoint.h"

class Renderable {
 public:
    virtual ~Renderable() { delete boundingBox; };
    AABB *getBoundingBox() { return boundingBox; };
    virtual IntersectionPoint *getIntersectionPoint(Ray *ray) const = 0;

 protected:
    AABB *boundingBox;
    virtual void createAABB() = 0;
};

#endif
