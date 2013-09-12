#ifndef AABB_H
#define AABB_H

#include <iostream>
#include <glm/glm.hpp>
#include "Ray.h"

class AABB {
public:
	AABB();
	AABB(const glm::vec3&, const glm::vec3&, const glm::vec3&);
	~AABB(){};
	bool isInside(const glm::vec3&);
	int getQuadrant(const glm::vec3&);
	float getIntersection(const Ray&);

	glm::vec3 getOrigin() const { return _origin; };
	glm::vec3 getLowerLeftBack() const { return _lowerLeftBack; };
	glm::vec3 getUpperRightFront() const { return _upperRightFront; }
private:
	glm::vec3 _origin;
	glm::vec3 _lowerLeftBack;
	glm::vec3 _upperRightFront;
};

#endif
