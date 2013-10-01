#ifndef PLANE_H
#define PLANE_H

#include "renderable.h"

class Plane : public Renderable
{
public:
	Plane(const glm::vec3& pos, const glm::vec3&n, const float w, const float h);
	~Plane() {};
	virtual void createAABB() {}; // Not implemented, trivial with plane
	virtual IntersectionPoint *getIntersectionPoint(Ray *ray) const;
	virtual std::string asString() const;
	glm::vec3 getPosition() const { return _position; }

private:
	glm::vec3 _position;
	glm::vec3 _normal;
	float width;
	float height;
};

#endif
