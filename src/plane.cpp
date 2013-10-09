#include "plane.h"

Plane::Plane(const glm::vec3& pos, const glm::vec3& n, const float w, const float h) {
	this->_position = pos;
	this->_normal = glm::normalize(n);
	this->width = w;
	this->height = h;
	this->_name = "plane";
}

std::string Plane::asString() const {
	std::stringstream s;
	s << _name << " p:["<<_position.x<<" "<<_position.y<<" "<<_position.z<<"] n:"<<_normal.x<<" "<<_normal.y<<" "<<_normal.z;
	return s.str();
}

bool Plane::getIntersectionPoint(Ray *ray, IntersectionPoint &ip) const {
	float nom, denom;
	glm::vec3 intVec;

	nom = glm::dot( (_position - ray->getOrigin()), _normal);
	denom = glm::dot( ray->getDirection(), _normal);

	if(fabs(denom) < 0.0000001f)
		return false; // Ray outside and/or parallel to plane

	float d = nom/denom;
	intVec = ray->getOrigin() + d * ray->getDirection();

	return false;
}
