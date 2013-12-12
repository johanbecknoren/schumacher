#ifndef AABB_H
#define AABB_H

#include <iostream>
//#include <glm/glm.hpp>
#include "Ray.h"
#include "intersectionpoint.h"

class AABB {
public:
	AABB();
	AABB(const Vec3&, const Vec3&, const Vec3& trash = Vec3(real(0)));
	~AABB(){};
	bool isInside(const Vec3&) const;
	int getQuadrant(const Vec3&) const;

	bool getIntersection(Ray&, IntersectionPoint&, bool = false) const;
	bool IntersectT(Ray *r, real *tmin, real *tmax) const;
	Vec3 getOrigin() const { return _origin; };
	Vec3 getLowerLeftBack() const { return _lowerLeftBack; }
	Vec3 getUpperRightFront() const { return _upperRightFront; }
	void setLowerLeftBack(const Vec3 &v) { _lowerLeftBack = v; }
	void setUpperRightFront(const Vec3 &v) { _upperRightFront = v; }
	void print() const;
private:
	Vec3 _origin;
	Vec3 _lowerLeftBack;
	Vec3 _upperRightFront;
};

#endif
