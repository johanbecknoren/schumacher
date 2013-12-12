#include "pointlight.h"

PointLight::PointLight(const Vec3 pos, 
				const real& intensity, 
				const Vec3 color) {
	_position = pos;
	_intensity = intensity;
	_color = color;
}
