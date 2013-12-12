#include "Ray.h"

Ray::Ray() {
	this->_origin = Vec3(real(0));
	this->_direction = Vec3(real(0));
}

Ray::Ray(const Vec3& o, const Vec3 &dir, const real rInd) {
	this->_origin = o;
	this->_direction = glm::normalize(dir);
	_tmin = real(-100000);
	_tmax = real(10000);
	_refractionIndex = rInd;
}

void Ray::updateT(const real tmin, const real tmax) { 
	_tmin = tmin; 
	_tmax = tmax; 
}
