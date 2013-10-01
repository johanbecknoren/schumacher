#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "renderable.h"

class PointLight : public Renderable {
public:
	PointLight(const glm::vec3 pos, 
					const float& intensity, 
					const glm::vec3 color);

	PointLight(const glm::vec3& lLB,
              const glm::vec3& uRF,
              const glm::vec3 p,
              const float& i, const glm::vec3 c);
	~PointLight() {};
	virtual void createAABB();
	virtual IntersectionPoint *getIntersectionPoint(Ray *ray) const;
	IntersectionPoint *getIntersection(Ray*, bool);
private:
	glm::vec3 _lowerLeftBack;
	glm::vec3 _upperRightFront;
	glm::vec3 _position;
	float _intensity;
	glm::vec3 _color;
};

#endif
