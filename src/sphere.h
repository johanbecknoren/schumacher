#ifndef SPHERE_H
#define SPHERE_H

#include "renderable.h"

class Sphere : public Renderable
{
public:
    Sphere(const real &radius, const Vec3 &position);
    ~Sphere() {};
	virtual void createAABB();
    virtual bool getIntersectionPoint(Ray &ray, IntersectionPoint &ip) const;
	virtual std::string asString() const;
	virtual std::string getName() const { return "Sphere"; };
	Vec3 getPosition() const { return _position; }
private:
	float _radius;
	Vec3 _position;

};

#endif
