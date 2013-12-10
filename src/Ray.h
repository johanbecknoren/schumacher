#ifndef RAY_H
#define RAY_H

#include <iostream>
#include "utils.h"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp> 
class Ray {
public:
	Ray();
	Ray(const Vec3& origin, const Vec3& dir, const real rInd = REFRACTION_AIR);
	~Ray(){};
	real getTMin() const { return _tmin; }
	real getTMax() const { return _tmax; }
	void setTMax(real tmax) { _tmax = tmax; }
	Vec3 getOrigin() const { return _origin; } ;
	Vec3 getPosition() const { return _origin * _tmin; };
	Vec3 getDirection() const { return glm::normalize(_direction); };
	float getRefractionIndex() const { return _refractionIndex; }
	void setOrigin(const Vec3 o) { _origin = o; };
	void setDirection(const Vec3 d) { _direction = glm::normalize(d); };
	void setRefractionIndex(const real r) { _refractionIndex = r; };
	void updateT(const real tmin, const real tmax);	
	Vec3 getPosition(const real t) const {
		return Vec3(_origin.x + t, _origin.y + t, _origin.z + t);
	}
	void print () {
		std::cout << "Ray - Origin: " << glm::to_string(_origin) << 
			"\nRay - Direction: " << glm::to_string(_direction) <<"\n";
	};
private:
	Vec3 _origin;
	real _tmin;
	real _tmax;
	real _refractionIndex;
	Vec3 _direction;
};

#endif
