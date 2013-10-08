#ifndef CAMERA_H
#define CAMERA_H
#include "Ray.h"

class Camera {
public:
	Camera() {
		_up = glm::vec3(0.0f,1.0f,0.0f);
		_direction = glm::vec3(0.0f,0.0f,-1.0f);
		_position = glm::vec3(0.0f,0.0f,-1.0f);
	}

	~Camera() {};
	glm::vec3 getUp() const { return _up; }
	glm::vec3 getDirection() const { return _direction; }
	glm::vec3 getPosition() const { return _position; }
	Ray createRay(float &x, float &y) { return Ray( _position, glm::vec3(x, y, -_direction.z)); }
	
private:
	glm::vec3 _up;
	glm::vec3 _direction;
	glm::vec3 _position;	
};

#endif
