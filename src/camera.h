#ifndef CAMERA_H
#define CAMERA_H
#include "Ray.h"

class Camera {
public:
	Camera() {
		_up = Vec3(real(0),real(0),real(0));
		_direction = Vec3(real(0),real(0),real(0));
		_position = Vec3(real(0),real(0),real(-1));
	}

	~Camera() {};
	Vec3 getUp() const { return _up; }
	Vec3 getDirection() const { return _direction; }
	Vec3 getPosition() const { return _position; }
	Ray createRay(real &x, real &y) const { return Ray( _position, Vec3(x, y, _direction.z)); }
	
private:
	Vec3 _up;
	Vec3 _direction;
	Vec3 _position;	
};

#endif
