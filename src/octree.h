#ifndef OCTREE_H
#define OCTREE_H
#include "renderable.h"

class Leaf {
public:
    Leaf(Renderable *renderable);
    ~Leaf();
    void setNextSibling(Leaf *sibling) { nextSibling = sibling; };
private:
    Renderable *renderable;
    Leaf *nextSibling;
};

class Node {
public:
    Node(int nodeDepth);
    ~Node();
    const AABB *getBoundingBox() const { return boundingBox; };
    Node *getChild(const int octant) { return childs[octant]; };
    const int getDepth() const { return depth; };
    void addLeaf(Node *parent, Renderable *renderable);
private:
    AABB *boundingBox;
    Node *parent;
    Node *childs[8];
    Node *nextSibling;
    Leaf *firstLeaf;
    int depth;
};



class Octree {
public:
    Octree();
    ~Octree();
    void addChild(Node *parent, int octant);
    void addLeaf(Renderable *renderable);
    void addObject(Renderable *object);
private:
    void subdivideBoundingBox(Node *parent, Renderable *object);
    Node *root;
};

#endif
