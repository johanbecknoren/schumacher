#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "renderable.h"

class PointLight {
public:
	PointLight(const glm::vec3 pos, 
					const float& intensity, 
					const glm::vec3 color);

	~PointLight() {};
	glm::vec3 getPosition() const { return _position; }
	glm::vec3 getColor() const { return _color; }
	glm::vec3 getIntensity() const { return _color; }
private:
	glm::vec3 _position;
	float _intensity;
	glm::vec3 _color;
};

#endif
