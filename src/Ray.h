#ifndef RAY_H
#define RAY_H

#include <iostream>
#include <glm/glm.hpp>

class Ray {
public:
	Ray();
	Ray(const glm::vec3&, const glm::vec3&);
	~Ray() {};

	
	
	glm::vec3 const getOrigin() { return this->_origin; };
	glm::vec3 const getDirection() { return this->_direction; };
private:
	glm::vec3 _origin;
	glm::vec3 _direction;
};

#endif
