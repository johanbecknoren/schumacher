#ifndef RAY_H
#define RAY_H

#include <iostream>
#include <glm/glm.hpp>

class Ray {
public:
	Ray();
	Ray(const glm::vec3&, const glm::vec3&);
	~Ray(){};
	
	glm::vec3 getOrigin() const { return _origin; };
	glm::vec3 getDirection() const { return _direction; };
private:
	glm::vec3 _origin;
	glm::vec3 _direction;
};

#endif
