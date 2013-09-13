#include "sphere.h"

Sphere::Sphere(const float &radius, const glm::vec3 &position) {
	_radius = radius;
	_position = position;
	createAABB();
}

void Sphere::createAABB() {
	glm::vec3 lLB(_position.x-_radius, _position.y-_radius, _position.z-_radius);
	glm::vec3 uRF(_position.x+_radius, _position.y+_radius, _position.z+_radius);
	this->boundingBox = new AABB(lLB, uRF, _position);
}
