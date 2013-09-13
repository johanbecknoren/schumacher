#include "octree.h"

Node::Node(int nodeDepth, AABB *nodeBoundingBox) {
	boundingBox = nodeBoundingBox;
	parent = NULL;
	depth = nodeDepth;
}

Leaf::Leaf(Renderable *object) {
	renderable = object;
}

void Node::addLeaf(Node *parent, Renderable *renderable) {
	Leaf *newLeaf = new Leaf(renderable);
	newLeaf->setNextSibling(parent->firstLeaf);
	parent->firstLeaf = newLeaf;
}

Octree::Octree(AABB *sceneBoundingBox) {
	root = new Node(0, sceneBoundingBox);
}

Octree::~Octree() {
	// TODO: Fix destructor
}

void Octree::addChild(Node *parent, int octant) {
	if(parent != NULL) {
	  Node *child = parent->getChild(octant);
	  if(child == NULL) {
		  child = new Node(parent->getDepth() + 1, createBoundingBox(parent, octant));
	  }
	}
}

void Octree::findIntersection(Ray *ray) const {
	ray = NULL;
}

void Octree::iterateRay(Ray *ray, Node *node) {
	if (node->getBoundingBox()->isInside(ray->getOrigin())) {
		Leaf *leaf = node->getFirstLeaf();
		while(leaf != NULL) {
			leaf->getRenderable()->getIntersectionPoint(ray);
		}
		for (int i = 0; i < 8; ++i) {
			if (node->getChild(i) != NULL) {
				iterateRay(ray, node->getChild(i));
			}
		}
	}
}

AABB *Octree::createBoundingBox(const Node *node, const int octant) {
	const AABB *box = node->getBoundingBox();
	glm::vec3 lowerLeft = box->getLowerLeftBack();
	glm::vec3 upperRight = box->getUpperRightFront();
	switch(octant) {
		case 0: {

		} break;
		default: {

		} break;
	}
	return new AABB(glm::vec3(-1), glm::vec3(1));
}

void Octree::addObject(Renderable *object) {
	// Start recursive add to find where in tree to add object
	subdivideBoundingBox(root, object);
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
	// FOR NOW, ADD ALL OBJECTS TO BASE OF TREE
	if(false) {
		addChild(parent, q1);
	}
	else {
		parent->addLeaf(parent, object);
	}
}
