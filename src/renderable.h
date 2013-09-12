#include "AABB.h"
class Renderable {
 public:
    Renderable();
    virtual ~Renderable();
    AABB *getBoundingBox() { return boundingBox; };
 private:
    AABB *boundingBox;
    virtual void createAABB() = 0;
};
