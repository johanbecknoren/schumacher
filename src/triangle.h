#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "renderable.h"

class Triangle : public Renderable {
public:
	Triangle(Vec3 x0, Vec3 x1, Vec3 x2);
	Triangle(Vec3 *x0, Vec3 *x1, Vec3 *x2);
	virtual ~Triangle() {};
	bool getIntersectionPoint(Ray &ray, IntersectionPoint &ip) const;
	void createAABB();
	void translate(const Vec3 &t);
	void scale(const real s);
	virtual std::string getName() const { return "Triangle"; };
	virtual std::string asString() const { return glm::to_string(x0) + " " + glm::to_string(x1) + " " + glm::to_string(x2); };
private:
	Vec3 x1;
	Vec3 x2;
	Vec3 x0;
	Vec3 normal;
};

#endif
