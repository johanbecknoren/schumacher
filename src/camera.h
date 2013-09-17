#ifndef CAMERA_H
#define CAMERA_H

class Camera {
public:
	Camera() {
		_up = glm::vec3(0.0f,1.0f,0.0f);
		_direction = glm::vec3(0.0f,0.0f,-1.0f);
		_position = glm::vec3(0.0f,0.0f,0.0f);
	}

	~Camera() {};
	glm::vec3 getUp() const { return _up; }
	glm::vec3 getDirection() const { return _direction; }
	glm::vec3 getPosition() const { return _position; }
	
	
private:
	glm::vec3 _up;
	glm::vec3 _direction;
	glm::vec3 _position;	
};

#endif
