#include "renderable.h"

class Node {
public:
    Node(int nodeDepth);
    ~Node();
    const AABB *getBoundingBox() const { return boundingBox; };
    Node *getChild(const int octant) { return childs[octant]; };
    const int getDepth() const { return depth };
private:
    AABB *boundingBox;
    Node *parent;
    Node *childs[8];
    Node *nextSibling;
    Leaf *firstLeaf;
    int depth;
};

class Leaf {
public:
    Leaf(Renderable *renderable);
    ~Leaf();
private:
    Renderable *renderable;
};

class Octree {
public:
    Octree();
    ~Octree();
    void addChild(Node *parent, int octant);
    void addLeaf(renderable);
    void addObject(Renderable *object);
private:
    void subdivideBoundingBox(const Node *parent, Renderable *object);
    Node *root;
}
