#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "renderable.h"

class Lightsource : public Renderable {
	Lightsource();
	Lightsource(const glm::vec3& lLB, const glm::vec3& uRF, const glm::vec3 p, const float& i, const glm::vec3 c);
	~Lightsource() {};
	virtual void createAABB();
	IntersectionPoint* getIntersection(Ray*, bool = false);
private:
	glm::vec3 _lowerLeftBack;
	glm::vec3 _upperRightFront;
	glm::vec3 _position;
	float _intensity;
	glm::vec3 _color;
};

#endif
