#include "AABB.h"
#include "intersectionpoint.h"

class Renderable {
 public:
    Renderable();
    virtual ~Renderable();
    AABB *getBoundingBox() { return boundingBox; };
    virtual IntersectionPoint *getIntersectionPoint(Ray *ray) const = 0;

 protected:
    AABB *boundingBox;
    virtual void createAABB() = 0;
};
