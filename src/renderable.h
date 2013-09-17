#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "AABB.h"
#include "intersectionpoint.h"

class Renderable {
 public:
    virtual ~Renderable() { delete _boundingBox; };
    AABB *getBoundingBox() { return _boundingBox; };
    virtual IntersectionPoint *getIntersectionPoint(Ray *ray) const = 0;
	std::string getName() const { return _name; };
	virtual std::string asString() const { return "asString() not implemented for " + getName(); };
 protected:
	std::string _name;
    AABB *_boundingBox;
    virtual void createAABB() = 0;
};

#endif
