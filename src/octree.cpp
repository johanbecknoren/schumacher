#include "octree.h"

Node::Node(int nodeDepth, AABB nodeBoundingBox) {
	_boundingBox = nodeBoundingBox;
	_parent = NULL;
	_depth = nodeDepth;
	_firstLeaf = NULL;
	_nextSibling = NULL;
	for(int i = 0; i < 8; ++i) {
		_childs[i] = NULL;
	}
}

Leaf::Leaf(Renderable *object) {
	_renderable = object;
	_nextSibling = NULL;
}

void Node::addLeaf(Leaf *leaf) {
	leaf->setNextSibling(NULL);
	_firstLeaf = leaf;
}

void Node::setChild(const int octant, Node* node) {
	_childs[octant] = node; 
}

Octree::Octree(AABB *sceneBoundingBox) {
	_root = new Node(0, *sceneBoundingBox);
	_nodes.push_back(*_root);
}

Octree::~Octree() {
	// TODO: Fix destructor
}

void Octree::print() const {
	std::cout << "Printing octree \n"; 
	for(int i = 0; i < _nodes.size(); ++i) {
		std::cout << _nodes[i].getDepth() << std::endl; 
	}
	for(int i = 0; i < _leafs.size(); ++i) {
		std::cout << _leafs[i].getRenderable()->getName() << std::endl; 
	}

	print(_root);
	std::cout << std::endl;
}

void Octree::print(Node *node) const {
	std::cout << "====== D" << node->getDepth() << " ======\n";
	Leaf *leaf = node->getFirstLeaf();
	if (leaf != NULL) {
		std::cout << "L:";
		do {
				const Renderable *r = leaf->getRenderable();
				std::cout << r->getName() << " ";
				leaf = leaf->getNextSibling();
		} while(leaf != NULL); 
	}
	for(int i = 0; i < 8; ++i) {
		Node *n = node->getChild(i);
		if (n != NULL)
		{
			print(n);
		}
	}
}

void Octree::addChild(Node *parent, int octant) {
	if(parent != NULL) {
		Node *child = new Node(parent->getDepth() + 1, *createBoundingBox(parent, octant));
		_nodes.push_back(*child);
		parent->setChild(octant, child);
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
	glm::vec3 diff = (upperRight - lowerLeft) / 2.0f;

	switch(octant) {
		case 0: {
		
		} break;
		case 1: {
			lowerLeft += glm::vec3(diff.x,
									0.0f,
									0.0f);	
		} break;
		case 2: {
			lowerLeft += glm::vec3(0.0f,
									0.0f,
									diff.z);	
		} break;
		case 3: {
			lowerLeft += glm::vec3(diff.x,
									0.0f,
									diff.z);
		}
		case 4: {
			lowerLeft += glm::vec3(0.0f,
									diff.y,
									0.0f);
		}
		case 5: {
			lowerLeft += glm::vec3(diff.x,
									diff.y,
									0.0f);	
		} break;
		case 6: {
			lowerLeft += glm::vec3(0.0f,
									diff.y,
									diff.z);	
		} break;
		case 7: {
			lowerLeft += glm::vec3(diff.x,
									diff.y,
									diff.z);
		}
		default: {
			
		} break;
	}
	upperRight = lowerLeft + diff;
	AABB *bb = new AABB(lowerLeft, upperRight);
	bb->print();
	return bb;
}

void Octree::addObject(Renderable *object) {
	// Start recursive add to find where in tree to add object
	subdivideBoundingBox(_root, object);
}
// Calculate sub bounding box in specified octant
void Octree::subdivideBoundingBox(Node *parent, Renderable *object) {
	std::cout << "Octree.h - Entering subdivide\n";
	const AABB *parentBox = parent->getBoundingBox();
	const AABB *objectBox = object->getBoundingBox();

	const glm::vec3 lowerLeft = objectBox->getLowerLeftBack();
	const glm::vec3 upperRight = objectBox->getUpperRightFront();
	objectBox->print();
	int q1 = parentBox->getQuadrant(lowerLeft);
	int q2 = parentBox->getQuadrant(upperRight);
	// If whole boundingbox is in same quadrant,
	// add node and continue subdividing.
	// FOR NOW, ADD ALL OBJECTS TO BASE OF TREE
	std::cout << "octree.cpp - q1:" << q1 << " q2:" << q2 << std::endl;
	if(q1 != -1 && q2 != -1 && q1 == q2) {
		addChild(parent, q1);
		std::cout << "octree.cpp - Added child in quadrant:"
					<< q1 << std::endl;
		subdivideBoundingBox(parent->getChild(q1), object);
	}
	else {
		std::cout << "octree.cpp - adding leaf\n";
		Leaf *leaf = new Leaf(object);
		_leafs.push_back(*leaf);
		parent->addLeaf(leaf);
	}
}
