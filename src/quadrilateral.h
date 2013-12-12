#ifndef QUADRILATERAL_H
#define QUADRILATERAL_H

#include "renderable.h"

class Quadrilateral : public Renderable {
public:
	
	Quadrilateral( const Vec3& v00, const Vec3& v10, const Vec3& v11, const Vec3& v01);
	~Quadrilateral() {};

	bool getIntersectionPoint(Ray &ray, IntersectionPoint &ip) const;

	virtual std::string asString() const;
	virtual std::string getName() const { return "Quadrilateral"; };

	Vec3 getRandomPoint(const real &s, const real &t, const real &randomFloat);
	real getArea();

	virtual void createAABB();
	
	void scale(const real v);
	void translate(const Vec3&);
private:
	Vec3 _v00;
	Vec3 _v01;
	Vec3 _v10;
	Vec3 _v11;
};

#endif
