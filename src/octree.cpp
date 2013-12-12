#include "octree.h"
// #include "sphere.h"
#include <climits>
#include "glmprint.h"
#define USE_OCTREE 1

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

void Octree::print(bool printLeaf) const {
	std::cout << "Printing octree \n";

	print(_root, printLeaf);
	std::cout << std::endl;
}

void Octree::print(Node *node, bool printLeaf) const {
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
	if (printLeaf) {
		Leaf *leaf = node->getFirstLeaf();
		if (leaf != NULL) {
			std::cout << " L:";
			do {
				const Renderable *r = leaf->getRenderable();
				std::cout << r->getName() << " ";
				leaf = leaf->getNextSibling();
			} while(leaf != NULL);
		}
	}
	std::cout << std::endl;
	for(int i = 0; i < 8; ++i) {
		Node *n = node->getChild(i);
		if (n != NULL) {
			print(n, printLeaf);
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
		if (r->getMaterial()->getMaterialType() == LIGHT) {
			lightList.push_back(r);
		}
	}
	return lightList;
}

bool Octree::intersect(Ray &ray, IntersectionPoint &isect) const {
	//return intersectSimple(ray, isect);
	return intersectHard(ray, isect);
}

bool Octree::intersectSimple(Ray &ray, IntersectionPoint &isect) const {
	real lMin = FLT_MAX;
	bool found = false;
	
	for (size_t i = 0; i < _leafs.size(); ++i) {
		IntersectionPoint ip;
		if (_leafs[i].getRenderable()->getIntersectionPoint(ray, ip)) {
			Vec3 vec = ip.getPoint() - ray.getOrigin();
			real len = glm::length((vec));
			if(len < lMin && len < ray.getTMax()) {
				lMin = len;
				isect = ip;
				found = true;
				
			}
		}
	}

	return found;
}



bool Octree::intersectHard(Ray &ray, IntersectionPoint &isect) const {
	real tmin = 0, tmax = FLT_MAX;
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
			IntersectionPoint i;
			if (leaf->getRenderable()->getIntersectionPoint(ray, i)) {
				pts.push_back(i);
				hit = true;
			}
			leaf = leaf->getNextSibling();
		}
		// Process interior nodes				
		int id = -1;
		real mint = FLT_MAX;
		real maxt = 0;
		for (int i = 0; i < 8; ++i) {
			if (node->getChild(i) != NULL) {

				real max, tplane;
				// If colliding with octree child boxes
				if (node->getChild(i)->getBoundingBox()->IntersectT(&ray, &tplane, &max)) {
					if(tplane < mint && tplane > 0)
					{
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
					else
					{
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
		real min = glm::distance(pts[0].getPoint(), ray.getOrigin());
		int id = 0;
		for (size_t i = 0; i < pts.size(); ++i) {
			// Vec3 vec = pts[i].getPoint() - ray.getOrigin();

			real len = glm::distance(ray.getOrigin(), pts[i].getPoint());

			if(len < min && len >= 0) {
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
	Vec3 lowerLeft = box->getLowerLeftBack();
	Vec3 upperRight = box->getUpperRightFront();
	Vec3 diff = (upperRight - lowerLeft) / real(2.0);

	switch(octant) {
		case 0: {

		} break;
		case 1: {
			lowerLeft += Vec3(diff.x, real(0.0), real(0.0));
		} break;
		case 2: {
			lowerLeft += Vec3(real(0.0), real(0.0), diff.z);
		} break;
		case 3: {
			lowerLeft += Vec3(diff.x, real(0.0), diff.z);
		} break;
		case 4: {
			lowerLeft += Vec3(real(0.0), diff.y, real(0.0));
		} break;
		case 5: {
			lowerLeft += Vec3(diff.x, diff.y, real(0.0));
		} break;
		case 6: {
			lowerLeft += Vec3(real(0.0), diff.y, diff.z);
		} break;
		case 7: {
			lowerLeft += Vec3(diff.x, diff.y, diff.z);
		}
		default: {
		} break;
	}
	upperRight = lowerLeft + diff;
	Vec3 origin = (lowerLeft + upperRight) / real(2.0);
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
	
	const Vec3 lowerLeft = objectBox->getLowerLeftBack();
	int q1 = parentBox->getQuadrant(lowerLeft);

	const Vec3 upperRight = objectBox->getUpperRightFront();
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
