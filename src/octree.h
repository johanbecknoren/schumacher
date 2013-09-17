#ifndef OCTREE_H
#define OCTREE_H
#include "renderable.h"
#include <vector>
class Leaf {
public:
    Leaf(Renderable *renderable);
    ~Leaf(){};
    void setNextSibling(Leaf *sibling) { _nextSibling = sibling; };
	Leaf *getNextSibling() { return _nextSibling; };
    const Renderable *getRenderable() const { return _renderable; };
private:
    Renderable *_renderable;
    Leaf *_nextSibling;
};

class Node {
public:
    Node(int nodeDepth, AABB boundingBox);
    ~Node(){};
    const AABB *getBoundingBox() const { return &_boundingBox; };
    Node *getChild(const int octant) { return _childs[octant]; };
	void setChild(const int octant, Node *node);
    const int getDepth() const { return _depth; };
    void addLeaf(Leaf *leaf);
    Leaf *getFirstLeaf() { return _firstLeaf; };
	void setFirstLeaf(Leaf *leaf) { _firstLeaf = leaf; }
private:
    AABB _boundingBox;
    Node *_parent;
    Node *_childs[8];
    Node *_nextSibling;
    Leaf *_firstLeaf;
    int _depth;
};

class Octree {
public:
    Octree(AABB *sceneBoundingBox);
    ~Octree();

    void addObject(Renderable *object);
	void print() const;
	void findIntersection(Ray *ray);

private:
	void addChild(Node *parent, int octant);
	void addLeaf(Renderable *renderable);
	IntersectionPoint *iterateRay(Ray *ray, Node *node, bool &active);
	AABB *createBoundingBox(const Node *node, const int octant);
	void subdivideBoundingBox(Node *parent, Renderable *object);
	void print(Node *node) const ;
	Node *_root;
	std::vector<Node> _nodes;
	std::vector<Leaf> _leafs;
};

#endif
