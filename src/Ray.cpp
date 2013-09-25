#include "Ray.h"

Ray::Ray() {
	this->_origin = glm::vec3(0.0f);
	this->_direction = glm::vec3(0.0f);
}

Ray::Ray(const glm::vec3& o, const glm::vec3 &dir) {
	this->_origin = o;
	this->_direction = glm::normalize(dir);
	_tmin = -100000;
	_tmax = 10000;
}

void Ray::updateT(const float tmin, const float tmax) { 
	_tmin = tmin; 
	_tmax = tmax; 
}
