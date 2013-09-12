#ifndef AABB_H
#define AABB_H

#include <iostream>
#include <glm/glm.hpp>

class AABB {
public:
	AABB();
	AABB(const glm::vec3&, const glm::vec3&, const glm::vec3&);
	~AABB();
	bool isInside(const glm::vec3&);
	float getIntersection(const glm::vec3&, const glm::vec3&);
private:
	glm::vec3 _origin;
	glm::vec3 _lowerLeftBack;
	glm::vec3 _upperRightFront;
};

#endif
