#include "octree.h"

Node::Node(int nodeDepth) {
    boundingBox = NULL;
    parent = NULL;
}


Octree::Octree() {
    root = new Node(0);
}

void Octree::addObject(Renderable *object) {
    // Get bounding box.
}
// Calculate sub bounding box in specified octant
void Octree::subdivideBoundingBox(const Node *parent, int octant) {
    const AABB *parentBox = parent->getBoundingBox();
    const glm::vec3 lowerLeft = parentBox->getLowerBack();
    const glm::vec3 upperRight = parentBox->getUpperRightFront();
    int q1 = parentBox->getQuadrant(lowerLeft);
    int q2 = parentBox->getQuadrant(upperRight);
    // check if bounding box is in quadrant
    if(q1 == q2) {
        
    }
}
