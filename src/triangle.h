#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "renderable.h"

class Triangle : public Renderable {
public:
	Triangle(glm::vec3 x0, glm::vec3 x1, glm::vec3 x2);
	Triangle(glm::vec3 *x0, glm::vec3 *x1, glm::vec3 *x2);
	virtual ~Triangle() {};
	bool getIntersectionPoint(Ray &ray, IntersectionPoint &ip) const;
	void createAABB();
	void translate(const glm::vec3 &t);
	void scale(const float s);
	virtual std::string getName() const { return "Triangle"; };
	virtual std::string asString() const { return glm::to_string(x0) + " " + glm::to_string(x1) + " " + glm::to_string(x2); };
private:
	glm::vec3 x1;
	glm::vec3 x2;
	glm::vec3 x0;
	glm::vec3 normal;
};

#endif
