#include "octree.h"
#include "sphere.h"
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
	leaf->setNextSibling(getFirstLeaf());
	setFirstLeaf(leaf);
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

	print(_root);
	std::cout << std::endl;
}

void Octree::print(Node *node) const {
	std::cout << "====== D" << node->getDepth() << " ";
	std::string s;
	for(int i = 0; i < 8; ++i) {
		Node *n = node->getChild(i);
		if (n != NULL) {
			s += '-';
		}
		else {
			s += '+';
		}
	}
	std::cout << s;
	Leaf *leaf = node->getFirstLeaf();
	if (leaf != NULL) {
		std::cout << " L:";
		do {
				const Renderable *r = leaf->getRenderable();
				std::cout << r->getName() << " ";
				leaf = leaf->getNextSibling();
		} while(leaf != NULL);
	}
	std::cout << std::endl;
	for(int i = 0; i < 8; ++i) {
		Node *n = node->getChild(i);
		if (n != NULL) {
			print(n);
		}
	}
}

void Octree::addChild(Node *parent, int octant) {
	if(parent != NULL) {
		Node *child = new Node(parent->getDepth() + 1,
                           *createBoundingBox(parent, octant));
		_nodes.push_back(*child);
		parent->setChild(octant, child);
	}
}

void Octree::findIntersection(Ray *ray) {
	std::string s;
	ray->print();
	std::cin >> s;
	if (!_root->getBoundingBox()->isInside(ray->getOrigin())) {
		IntersectionPoint *ip = _root->getBoundingBox()->getIntersection(ray);
		Ray *r = new Ray(ip->getPoint(), ray->getDirection());
		iterateRay(r, _root);
	}
	else {
		iterateRay(ray, _root);
	}
}

bool Octree::iterateRay(Ray *ray, Node *node) {
	if (node->getBoundingBox()->isInside(ray->getOrigin())) {
		Leaf *leaf = node->getFirstLeaf();
		while(leaf != NULL) {
			IntersectionPoint *i = leaf->getRenderable()->getIntersectionPoint(ray);
			if (i != NULL) {
				std::cout << "Intersection found at: " << i->getPoint().x
						<< " " << i->getPoint().y << " "
						<< i->getPoint().z << std::endl;
			}
			leaf = leaf->getNextSibling();
		}
		for (int i = 0; i < 8; ++i) {
			if (node->getChild(i) != NULL) {
				std::cout << "octree.cpp - oct: " << i << " d:" << node->getDepth()  << " \n";
				iterateRay(ray, node->getChild(i));
			}
		}
	}
	else {
		std::cout << "octree.cpp - Break at depth: " << node->getDepth() << " No collision\n";
		std::cout << "Hej " << node->getDepth() << std::endl;
		std::vector<IntersectionPoint *> pts;
		for (int p = 0; p < 8; ++p) {
			if (node->getChild(p) != NULL) {
				if (!node->getChild(p)->getBoundingBox()->isInside(ray->getOrigin())) {
					IntersectionPoint *ip =
						node->getChild(p)->getBoundingBox()->getIntersection(ray);
					if (ip != NULL) {
						pts.push_back(ip);
					}
				}
			}				
		}
		for (int p = 0; p < pts.size(); ++p) {
			std::cout << pts[p]->asString() << " | ";
			Ray *r = new Ray(pts[p]->getPoint() + 0.000001f * ray->getDirection(),
									ray->getDirection());
			findIntersection(r);
			
		}

		return false;
	}
	return false;
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
	glm::vec3 origin = (lowerLeft + upperRight) / 2.0f;
	AABB *bb = new AABB(lowerLeft, upperRight, origin);
	return bb;
}

void Octree::addObject(Renderable *object) {
	// Start recursive add to find where in tree to add object
	subdivideBoundingBox(_root, object);
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
	if(q1 != -1 && q2 != -1 && q1 == q2) {
		if(parent->getChild(q1) == NULL)
			addChild(parent, q1);
		subdivideBoundingBox(parent->getChild(q1), object);
	}
	else {
		Leaf *leaf = new Leaf(object);
		_leafs.push_back(*leaf);
		parent->addLeaf(leaf);
	}
}
