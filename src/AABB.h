#ifndef AABB_H
#define AABB_H

#include <iostream>
#include <glm/glm.hpp>
#include "Ray.h"
#include "intersectionpoint.h"

class AABB {
public:
	AABB();
	AABB(const glm::vec3&, const glm::vec3&, const glm::vec3& trash = glm::vec3(0.0f));
	~AABB(){};
	bool isInside(const glm::vec3&) const;
	int getQuadrant(const glm::vec3&) const;
	bool getIntersection(Ray&, IntersectionPoint &point, bool = false) const;
	bool IntersectT(Ray *r, float *tmin, float *tmax) const;
	glm::vec3 getOrigin() const { return _origin; };
	glm::vec3 getLowerLeftBack() const { return _lowerLeftBack; };
	glm::vec3 getUpperRightFront() const { return _upperRightFront; }
	void setLowerLeftBack(const glm::vec3 &v) { _lowerLeftBack = v; }
	void setUpperRightFront(const glm::vec3 &v) { _upperRightFront = v; }
	void print() const;
private:
	glm::vec3 _origin;
	glm::vec3 _lowerLeftBack;
	glm::vec3 _upperRightFront;
};

#endif
