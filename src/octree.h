#include "renderable.h"

class Node {
public:
    Node(int nodeDepth);
    ~Node();
    const AABB *getBoundingBox() const { return boundingBox; };
private:
    AABB *boundingBox;
    Node *parent;
    Node *firstChild;
    Node *nextSibling;
    Leaf *firstLeaf;
    int depth;
};

class Leaf {
public:
    Leaf();
    ~Leaf();
private:
    Renderable *renderable;
};

class Octree {
public:
    Octree();
    ~Octree();
    void addChild(Node *parent);

private:
    void subdivideBoundingBox(const Node *parent, int octant);
    Node *root;
}
