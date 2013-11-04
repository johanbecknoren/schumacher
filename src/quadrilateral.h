#ifndef QUADRILATERAL_H
#define QUADRILATERAL_H

#include "renderable.h"

class Quadrilateral : public Renderable {
public:
	Quadrilateral( const glm::vec3& v00, const glm::vec3& v10, const glm::vec3& v11, const glm::vec3& v01);
	Quadrilateral() {};
	~Quadrilateral() {};

	bool getIntersectionPoint(Ray &ray, IntersectionPoint &ip) const;

	virtual std::string asString() const;

	virtual void createAABB();

	void scale(float v);
	void translate(const glm::vec3&);
private:
	glm::vec3 _v00;
	glm::vec3 _v01;
	glm::vec3 _v10;
	glm::vec3 _v11;
};

#endif
