#ifndef SPHERE_H
#define SPHERE_H

#include "renderable.h"

class Sphere : Renderable
{
public:
    Sphere(const float &radius, const glm::vec3 &position);
	~Sphere() {};
	virtual void createAABB();
private:
	float _radius;
	glm::vec3 _position;
};

#endif
