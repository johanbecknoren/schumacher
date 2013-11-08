#ifndef MATERIAL_H
#define MATERIAL_H

#include "utils.h"
#include "glm/glm.hpp"

class Material {
public:
	Material() {
		//default material (stone-isch)
		_diffuseColor = glm::vec3(0.f,0.f,0.f); // black
		_refractionIndex = 1.0f; // almost none (float precision)
		_specular = 0.01f; // diffuse (low specularity)
		_opacity = 1.0f; // completely solid
		_emission = glm::vec3(0.0f);
		_absorbtion = 0.75f;
	}
	Material(const glm::vec3 &d, const float &rI, const float &s, const float &o) {
		_diffuseColor = d;
		_refractionIndex = rI;
		_specular = s;
		_opacity = o;
	}
	~Material() {};

	glm::vec3 getDiffuseColor() const { return _diffuseColor; }
	float getRefractionIndex() const { return _refractionIndex; }
	float getSpecular() const { return _specular; }
	float getOpacity() const { return _opacity; }
	glm::vec3 getEmission() const { return _emission; }
	material_t getMaterialType() const { return _materialType; }
	float getAbsorbtion() const { return _absorbtion; }

	void setDiffuseColor(const glm::vec3& d) { _diffuseColor = d; }
	void setRefractionIndex(const float& ri) { _refractionIndex = ri; }
	void setSpecular(const float& s) { _specular = s; }
	void setOpacity(const float& o) { _opacity = o; }
	void setEmission(const glm::vec3 e) { _emission = e; }
	void setMaterialType(material_t type) { _materialType = type; }
	void setAbsorbtion(float absorbtion) { _absorbtion = absorbtion; }

private:
	material_t _materialType;
	glm::vec3 _diffuseColor;
	float _refractionIndex;
	float _absorbtion;
	float _specular;
	float _opacity;	
	glm::vec3 _emission;
};

#endif
