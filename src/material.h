#ifndef MATERIAL_H
#define MATERIAL_H

class Material {
public:
	Material() {
		//default material
		_color = glm::vec3(1.0f,0.0f,0.0f);
		_refractionIndex = 0.0f;
		_diffuse = 0.9f;
		_specular = 0.1f;
		_opacity = 1.0f;
	}
	Material(const glm::vec3 &c, const float &rI, const float& d, const float &s, const float &o) {
		_color = c;
		_refractionIndex = rI;
		_diffuse = d;
		_specular = s;
		_opacity = o;
	}
	~Material() {};

	glm::vec3 getColor() const { return _color; }
	float getRefractionIndex() const { return _refractionIndex; }
	float getDiffuse() const { return _diffuse; }
	float getSpecular() const { return _specular; }
	float getOpacity() const { return _opacity; }

	void setColor(const glm::vec3& c) { _color = c; }
	void setRefractionIndex(const float& rI) { _refractionIndex = rI; }
	void setDiffuse(const float& d) { _diffuse = d; }
	void setSpecular(const float& s) { _specular = s; }
	void setOpacity(const float& o) { _opacity = o; }

private:
	glm::vec3 _color;
	float _refractionIndex;
	float _diffuse;
	float _specular;
	float _opacity;	
};

#endif
