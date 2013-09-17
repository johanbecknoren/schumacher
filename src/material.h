#ifndef MATERIAL_H
#define MATERIAL_H

class Material {
public:
	Material() {
		//default material
		_diffuseColor = glm::vec3(0.5f,0.0f,0.0f); // gray
		_refractionIndex = 1.0f; // almost none (float precision)
		_specular = 0.2f; // diffuse (low specularity)
		_opacity = 1.0f; // completely solid
		_emission = glm::vec3(0.0f);
	}
	Material(const glm::vec3 &c, const float &rI, const float &s, const float &o) {
		_diffuseColor = c;
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

	void setDiffuseColor(const glm::vec3& c) { _diffuseColor = c; }
	void setRefractionIndex(const float& rI) { _refractionIndex = rI; }
	void setSpecular(const float& s) { _specular = s; }
	void setOpacity(const float& o) { _opacity = o; }
	void setEmission(const glm::vec3 e) { _emission = e; }

private:
	glm::vec3 _diffuseColor;
	float _refractionIndex;
	float _specular;
	float _opacity;	
	glm::vec3 _emission;
};

#endif
