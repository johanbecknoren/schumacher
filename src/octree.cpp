#include "octree.h"

Node::Node(int nodeDepth) {
    boundingBox = NULL;
    parent = NULL;
}

Leaf::Leaf(Renderable *object) {
    renderable = object;
}

void Node::addLeaf(Node *parent, Renderable *renderable) {
    Leaf *newLeaf = new Leaf(renderable);
    newLeaf->setNextSibling(parent->firstLeaf);
    parent->firstLeaf = newLeaf;
}

Octree::Octree() {
    root = new Node(0);
}

void Octree::addChild(Node *parent, int octant) {
    Node *child = parent->getChild(octant);
    if(child == NULL) {
        child = new Node(parent->getDepth() + 1);
    }
}

void createBoundingBox(const Node *node, const int octant) {
    const AABB *box = node->getBoundingBox();
    glm::vec3 lowerLeft;
    glm::vec3 upperRight;
    switch(octant) {
        case 0: {

        } break;
        default: {

        } break;
    }
}

void Octree::addObject(Renderable *object) {
    // Get bounding box.
}
// Calculate sub bounding box in specified octant
void Octree::subdivideBoundingBox(Node *parent, Renderable *object) {
    const AABB *parentBox = parent->getBoundingBox();
    const AABB *objectBox = object->getBoundingBox();

    const glm::vec3 lowerLeft = objectBox->getLowerLeftBack();
    const glm::vec3 upperRight = objectBox->getUpperRightFront();

    int q1 = parentBox->getQuadrant(lowerLeft);
    int q2 = parentBox->getQuadrant(upperRight);
    // If whole boundingbox is in same quadrant,
    // add node and continue subdividing.
    if(q1 == q2) {
        addChild(parent, q1);
        
    }
    else {
        parent->addLeaf(parent, object);
    }
}
