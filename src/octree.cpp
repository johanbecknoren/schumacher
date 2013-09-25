#include "octree.h"
#include "sphere.h"
#include <climits>

#define USE_OCTREE

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

IntersectionPoint *Octree::findIntersection(Ray *ray) {
	std::string s;
	//ray->print();
	//std::cin >> s;
	bool b;
	if (!_root->getBoundingBox()->isInside(ray->getOrigin())) {
		IntersectionPoint *ip = _root->getBoundingBox()->getIntersection(ray);
		Ray *r = new Ray(ip->getPoint(), ray->getDirection());
		
		return iterateRay(r, _root, b);
	}
	else {
		return iterateRay(ray, _root, b);
	}
}

std::vector<const Renderable*> Octree::getLightList() const {
	std::vector<const Renderable*> lightList;
	for (int i = 0; i < _leafs.size(); ++i) {
		const Renderable *r = _leafs[i].getRenderable();
		if (r->getMaterial().getMaterialType() == LIGHT) {
			lightList.push_back(r);
		}
	}
	return lightList;
}
bool Octree::intersect(Ray &ray, IntersectionPoint &isect) {
	float tmin = 0, tmax = FLT_MAX;
	if (!_root->getBoundingBox()->IntersectT(&ray, &tmin, &tmax))
		return false;
	
	// Prepare for traversal
	glm::vec3 invDir = 1.0f / ray.getDirection();
#define MAX_TODO 64
	ToDo todo[MAX_TODO];
	int todoPos = 0;
	bool hit = false;
	const Node *node = _root;
	std::vector<IntersectionPoint> pts;
	while (node != NULL) {
		// Exit if we found hit closer
		if (ray.getTMax() < tmin) break;
		// Check intersections inside leaf node
		Leaf *leaf = node->getFirstLeaf();

		while(leaf != NULL) {
			IntersectionPoint *i = leaf->getRenderable()->getIntersectionPoint(&ray);
			if (i != NULL) {
				pts.push_back(*i);
				hit = true;
			}
			leaf = leaf->getNextSibling();
		}
		// Process interior nodes				
		int id = -1;
		float mint = FLT_MAX;
		float maxt = 0;
		for (int i = 0; i < 8; ++i) {
			if (node->getChild(i) != NULL) {

				float max, tplane;
				if (node->getChild(i)->getBoundingBox()->IntersectT(&ray, &tplane, &max)) {
					if(tplane < mint) {
						if (id != -1) {
							todo[todoPos].node = node->getChild(i);
							todo[todoPos].tmin = mint;
							todo[todoPos].tmax = maxt;
							++todoPos;
						}
						id = i;
						mint = tplane;
						maxt = max;
					}
					else {
						todo[todoPos].node = node->getChild(i);
						todo[todoPos].tmin = tplane;
						todo[todoPos].tmax = max;
						++todoPos;
					}
				}
			}
		}

		if (id == -1) {
			// Grab next node to process
			if (todoPos > 0 ) {
				--todoPos;
				node = todo[todoPos].node;
				tmin = todo[todoPos].tmin;
				tmax = todo[todoPos].tmax;
			}
			else {
				
				break;
			}
		}
		else {
			node = node->getChild(id);
			tmin = mint;
			tmax = maxt;
		}
	}
	// Find closest intersection
	if (pts.size() > 0) {
		float min = FLT_MAX;
		int id;
		for (int i = 0; i < pts.size(); ++i) {
			float len = glm::length((pts[i].getPoint() - ray.getOrigin()));

			if(len < min) {
				min = len;
				id = i;
			}
		}
		isect = pts[id];
	}

	return hit;	
}
IntersectionPoint *Octree::iterateRay(Ray *ray, Node *node, bool &active) {
	bool findNew = false;
	if (node->getBoundingBox()->isInside(ray->getOrigin())) {
		//std::cout << "Inside\n";
		Leaf *leaf = node->getFirstLeaf();
		std::vector<IntersectionPoint> pts;
		while(leaf != NULL) {
			//std::cout << "Testing leaf "<< leaf->getRenderable()->asString() << " \n";
			IntersectionPoint *i = leaf->getRenderable()->getIntersectionPoint(ray);
			if (i != NULL) {
				//std::cout << "Intersection found at: " << i->getPoint().x
						//<< " " << i->getPoint().y << " "
					//	<< i->getPoint().z << std::endl;
				active = true;
				pts.push_back(*i);
			}
			
			leaf = leaf->getNextSibling();
		}
		if (pts.size() > 0) {
			float min = FLT_MAX;
			int id;
			for (int i = 0; i < pts.size(); ++i) {
				float len = glm::length((pts[i].getPoint() - ray->getOrigin()));

				if(len < min) {
					min = len;
					id = i;
				}
			}
			return &pts[id];
		}
		for (int i = 0; i < 8; ++i) {
			if (node->getChild(i) != NULL) {
			//	std::cout << "octree.cpp - oct: " << i << " d:" << node->getDepth()  << " \n";
				bool act;
				IntersectionPoint *ip = iterateRay(ray, node->getChild(i), act);
				if (ip != NULL) return ip;	
				if (!act) {
					findNew = true;
				}
			}
		}
	}
	else { 
		findNew = true;
		std::cout << "Outside of ";
		node->getBoundingBox()->print();
	}
	if (findNew) {
		//std::cout << "octree.cpp - Break at depth: " << node->getDepth() << " " << " No collision\n";
		node->getBoundingBox()->print();
		//std::cout << "Hej " << node->getDepth() << std::endl << std::endl;
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
			ray->print();
			std::cout << pts[p]->asString() << " | ";
			Ray *r = new Ray(pts[p]->getPoint() + ray->getDirection() * 0.00001f,
									ray->getDirection());
			findIntersection(r);
			
		}
		active = false;
		return NULL;
	}
	active = true;
	return NULL;
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
			lowerLeft += glm::vec3(diff.x, 0.0f, 0.0f);
		} break;
		case 2: {
			lowerLeft += glm::vec3(0.0f, 0.0f, diff.z);
		} break;
		case 3: {
			lowerLeft += glm::vec3(diff.x, 0.0f, diff.z);
		} break;
		case 4: {
			lowerLeft += glm::vec3(0.0f, diff.y, .0f);
		} break;
		case 5: {
			lowerLeft += glm::vec3(diff.x, diff.y, .0f);
		} break;
		case 6: {
			lowerLeft += glm::vec3(0.0f, diff.y, diff.z);
		} break;
		case 7: {
			lowerLeft += glm::vec3(diff.x, diff.y, diff.z);
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
#ifdef USE_OCTREE
	if(q1 != -1 && q2 != -1 && q1 == q2) {
#else
	if(false) {
#endif
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
