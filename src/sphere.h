#ifndef SPHERE_H
#define SPHERE_H

#include "renderable.h"

class Sphere : public Renderable
{
public:
    Sphere(const float &radius, const glm::vec3 &position);
    ~Sphere() {};
	virtual void createAABB();
    virtual bool getIntersectionPoint(Ray *ray, IntersectionPoint &ip) const;
	virtual std::string asString() const;
	glm::vec3 getPosition() const { return _position; }
private:
	float _radius;
	glm::vec3 _position;

};

#endif
