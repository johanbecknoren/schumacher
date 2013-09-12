#include "AABB.h"
class Renderable {
 public:
    Renderable();
    virtual ~Renderable();
    AABB *getBoundingBox() { return boundingBox; };
 protected:
    AABB *boundingBox;
    virtual void createAABB() = 0;
};
