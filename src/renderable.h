#include "AABB.h"
class Renderable {
 public:
    Renderable();
    virtual ~Renderable();
 private:
    AABB *boundingBox;

};
