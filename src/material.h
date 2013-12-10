#ifndef MATERIAL_H
#define MATERIAL_H

#include "utils.h"
#include "glm/glm.hpp"

class Material {
public:
	Material() {
		//default material (stone-isch)
		_diffuseColor = Vec3(real(0),real(0),real(0)); // black
		_refractionIndex = 1.0f; // almost none (float precision)
		_specular = 0.0f; // diffuse (low specularity)
		_opacity = 1.0f; // completely solid
		_emission = Vec3(0.0f);
		_absorbtion = 0.5f;
	}
	Material(const Vec3 &d, const real &rI, const real &s, const real &o) {
		_diffuseColor = d;
		_refractionIndex = rI;
		_specular = s;
		_opacity = o;
	}
	~Material() {};

	Vec3 getDiffuseColor() const { return _diffuseColor; }
	real getRefractionIndex() const { return _refractionIndex; }
	real getSpecular() const { return _specular; }
	real getOpacity() const { return _opacity; }
	Vec3 getEmission() const { return _emission; }
	material_t getMaterialType() const { return _materialType; }
	real getAbsorbtion() const { return _absorbtion; }

	void setDiffuseColor(const Vec3& d) { _diffuseColor = d; }
	void setRefractionIndex(const real& ri) { _refractionIndex = ri; }
	void setSpecular(const real& s) { _specular = s; }
	void setOpacity(const real& o) { _opacity = o; }
	void setEmission(const Vec3 e) { _emission = e; }
	void setMaterialType(material_t type) { _materialType = type; }
	void setAbsorbtion(real absorbtion) { _absorbtion = absorbtion; }

private:
	material_t _materialType;
	Vec3 _diffuseColor;
	real _refractionIndex;
	real _absorbtion;
	real _specular;
	real _opacity;	
	Vec3 _emission;
};

#endif
