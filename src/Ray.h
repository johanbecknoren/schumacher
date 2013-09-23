#ifndef RAY_H
#define RAY_H

#include <iostream>
#include <glm/glm.hpp>

class Ray {
public:
	Ray();
	Ray(const glm::vec3&, const glm::vec3&);
	~Ray(){};
	float getTMin() const { return _tmin; }
	float getTMax() const { return _tmax; }
	glm::vec3 getOrigin() const { return _origin; } ;
	glm::vec3 getPosition() const { return _origin * _tmin; };
	glm::vec3 getDirection() const { return _direction; };
	void setOrigin(const glm::vec3 o) { _origin = o; };
	void setDirection(const glm::vec3 d) { _direction = glm::normalize(d); };
	void updateT(const float tmin, const float tmax);	
	void print () {
		std::cout << "Ray - Origin: " << _origin.x << ',' << _origin.y << ','
        << _origin.z << "\nRay - Direction: " << _direction.x << ','
        << _direction.y<<','<<_direction.z<<"\n";
	};
private:
	glm::vec3 _origin;
	float _tmin;
	float _tmax;
	glm::vec3 _direction;
};

#endif
