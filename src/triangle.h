#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "renderable.h"

class Triangle : public Renderable {
public:
	Triangle(glm::vec3 x0, glm::vec3 x1, glm::vec3 x2);
	
	bool getIntersectionPoint(Ray &ray, IntersectionPoint &ip) const;
	void createAABB();
	void translate(const glm::vec3 &t);
	void scale(const float s);
	virtual std::string getName() const { return "Triangle"; };
private:
	glm::vec3 x1;
	glm::vec3 x2;
	glm::vec3 x0;
	glm::vec3 normal;
};

#endif
