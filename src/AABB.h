#ifndef AABB_H
#define AABB_H

#include <iostream>
#include <glm/glm.hpp>
#include "Ray.h"
#include "intersectionpoint.h"

class AABB {
public:
	AABB();
	AABB(const glm::vec3&, const glm::vec3&, const glm::vec3 o = glm::vec3(0.0f));
	~AABB(){};
	bool isInside(const glm::vec3&) const;
	int getQuadrant(const glm::vec3&) const;
	IntersectionPoint* getIntersection(Ray*, bool = false) const;
	bool IntersectT(Ray *r, float *tmin, float *tmax) const;
	glm::vec3 getOrigin() const { return _origin; };
	glm::vec3 getLowerLeftBack() const { return _lowerLeftBack; };
	glm::vec3 getUpperRightFront() const { return _upperRightFront; }
	void print() const;
private:
	glm::vec3 _origin;
	glm::vec3 _lowerLeftBack;
	glm::vec3 _upperRightFront;
};

#endif
