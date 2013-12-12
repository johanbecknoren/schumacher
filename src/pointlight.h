#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "renderable.h"

class PointLight {
public:
	PointLight(const Vec3 pos, 
					const real& intensity, 
					const Vec3 color);

	~PointLight() {};
	Vec3 getPosition() const { return _position; }
	Vec3 getColor() const { return _color; }
	Vec3 getIntensity() const { return _color; }
private:
	Vec3 _position;
	real _intensity;
	Vec3 _color;
};

#endif
