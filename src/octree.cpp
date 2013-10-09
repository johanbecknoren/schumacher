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
	for(size_t i = 0; i < 8; ++i) {
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

std::vector<const Renderable*> Octree::getLightList() const {
	std::vector<const Renderable*> lightList;
	for (size_t i = 0; i < _leafs.size(); ++i) {
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
		for (size_t i = 0; i < pts.size(); ++i) {
// 			std::cout << glm::to_string(pts[i].getPoint()) << glm::to_string(ray.getOrigin()) << std::flush;
			glm::vec3 vec = pts[i].getPoint() - ray.getOrigin();
			float len = glm::length((vec));

			if(len < min && glm::dot(vec, ray.getDirection()) >= 0.f) {
				
				min = len;
				id = i;
			}
		}
		isect = IntersectionPoint(pts[id]);
	}

	return hit;	
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

#ifdef USE_OCTREE	
	// If whole boundingbox is in same quadrant,
	// add node and continue subdividing.
	const AABB *parentBox = parent->getBoundingBox();
	const AABB *objectBox = object->getBoundingBox();	
	
	const glm::vec3 lowerLeft = objectBox->getLowerLeftBack();
	int q1 = parentBox->getQuadrant(lowerLeft);

	const glm::vec3 upperRight = objectBox->getUpperRightFront();
	int q2 = parentBox->getQuadrant(upperRight);

	if(q1 != -1 && q2 != -1 && q1 == q2) {		
		if(parent->getChild(q1) == NULL) {
			addChild(parent, q1);
		}
		subdivideBoundingBox(parent->getChild(q1), object);
	}
	else {
#else
	// We are not using octree, so we dont need to do anything 
	// with the bounding boxes when adding objects.
	{
#endif
		Leaf *leaf = new Leaf(object);
		_leafs.push_back(*leaf);
		parent->addLeaf(leaf);
	}
}
