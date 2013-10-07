#ifndef RAY_H
#define RAY_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp> 
class Ray {
public:
	Ray();
	Ray(const glm::vec3& origin, const glm::vec3& dir);
	~Ray(){};
	float getTMin() const { return _tmin; }
	float getTMax() const { return _tmax; }
	void setTMax(float tmax) { _tmax = tmax; }
	glm::vec3 getOrigin() const { return _origin; } ;
	glm::vec3 getPosition() const { return _origin * _tmin; };
	glm::vec3 getDirection() const { return _direction; };
	void setOrigin(const glm::vec3 o) { _origin = o; };
	void setDirection(const glm::vec3 d) { _direction = glm::normalize(d); };
	void updateT(const float tmin, const float tmax);	
	glm::vec3 getPosition(const float t) const {
		return glm::vec3(_origin.x + t, _origin.y + t, _origin.z + t);
	}
	void print () {
		std::cout << "Ray - Origin: " << glm::to_string(_origin) << 
			"\nRay - Direction: " << glm::to_string(_direction) <<"\n";
	};
private:
	glm::vec3 _origin;
	float _tmin;
	float _tmax;
	glm::vec3 _direction;
};

#endif
