#ifndef MATERIAL_H
#define MATERIAL_H

class Material {
public:
	Material(const glm::vec3 &c, const float &rI, const float &s, const float &o) {
		_color = c;
		_refractionIndex = rI;
		_specular = s;
		_opacity = o;
	}
	~Material() {};

	glm::vec3 getColor() const { return _color; }
	float getRefractionIndex() const { return _refractionIndex; }
	float getSpecular() const { return _specular; }
	float getOpacity() const { return _opacity; }

	void setColor(const glm::vec3& c) { _color = c; }
	void setRefractionIndex(const float& rI) { _refractionIndex = rI; }
	void setSpecular(const float& s) { _specular = s; }
	void setOpacity(const float& o) { _opacity = o; }

private:
	glm::vec3 _color;
	float _refractionIndex;
	float _specular;
	float _opacity;	
};

#endif
