#include "lightsource.h"

Lightsource::Lightsource(const glm::vec3& lLB, const glm::vec3& uRF, const glm::vec3 p, const float& i, const glm::vec3 c) {
	_lowerLeftBack = lLB;
	_upperRightFront = uRF;
	_position = p;
	_intensity = i;
	_color = c;
	_name = "Lightsource";

	createAABB();
}

void Lightsource::createAABB() {
	this->_boundingBox = new AABB(_lowerLeftBack, _upperRightFront, _position);
}

IntersectionPoint* Lightsource::getIntersection(Ray *r, bool getIntersectionNormals) {
	return this->_boundingBox->getIntersection(r,getIntersectionNormals);
}
