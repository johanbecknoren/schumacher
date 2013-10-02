#ifndef QUADRILATERAL_H
#define QUADRILATERAL_H

#include "renderable.h"

class Quadrilateral : public Renderable {
public:
	Quadrilateral( const glm::vec3& v00, const glm::vec3& v01, const glm::vec3& v10, const glm::vec3& v11);
	Quadrilateral() {};
	~Quadrilateral() {};

	IntersectionPoint *getIntersectionPoint(Ray *ray) const;

	virtual std::string asString() const;

	virtual void createAABB();

	void scale(float v);
private:
	glm::vec3 _v00;
	glm::vec3 _v01;
	glm::vec3 _v10;
	glm::vec3 _v11;
};

#endif
