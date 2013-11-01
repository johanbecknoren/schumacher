#include "pointlight.h"

PointLight::PointLight(const glm::vec3 pos, 
				const float& intensity, 
				const glm::vec3 color) {
	_position = pos;
	_intensity = intensity;
	_color = color;
}
