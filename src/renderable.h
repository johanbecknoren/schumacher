#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "AABB.h"
#include "intersectionpoint.h"
#include "utils.h"

class Renderable {
 public:
    virtual ~Renderable() { delete _boundingBox; };
    AABB *getBoundingBox() { return _boundingBox; };
    virtual bool getIntersectionPoint(Ray *ray,  IntersectionPoint &ip) const = 0;
	std::string getName() const { return _name; };
	virtual std::string asString() const { return "asString() not implemented for " + getName(); };
	virtual void scale(const float s) {
		std::cout << "Using unimplemented scale in " << getName() << std::endl;
	}
	virtual void translate(const glm::vec3 &t) {
		std::cout << "Using unimplemented translate in " << getName() << std::endl;

	}
	void setMaterial(const material_t& m) {
		_material = Material();
		_material.setMaterialType(m);
		if (m == GLASS) {
			_material.setDiffuseColor(glm::vec3(0.0f, 0.0f, 1.0f));
			_material.setRefractionIndex(REFRACTION_GLASS);
			_material.setOpacity(0.03);
			_material.setSpecular(1.0f);
		} else if (m == MARBLE) {
			_material.setDiffuseColor(glm::vec3(1.0f, 1.0f,0.0f));
			_material.setSpecular(0.8f);
			_material.setAbsorbtion(0.75f);
		} else if(m == LIGHT) {
			_material.setDiffuseColor(glm::vec3(0.9f));
			_material.setEmission(glm::vec3(1.0f));
		} else if (m == MIRROR) {
			_material.setDiffuseColor(glm::vec3(0.2f, 0.2f, 1.f));
			_material.setAbsorbtion(0.1f);
			_material.setSpecular(0.8f);
		} else if (m == STONE) {	
			_material.setDiffuseColor(glm::vec3(0.8f,0.8f,0.8f)); // gray
		} else if (m == CORNELL_CEIL) {
			_material.setDiffuseColor(glm::vec3(1.0f));
			_material.setAbsorbtion(1.f);
		} else if (m == CORNELL_FLOOR) {
			_material.setDiffuseColor(glm::vec3(1.0f));
			_material.setAbsorbtion(0.9f);
		} else if (m == CORNELL_BACK) {
			_material.setDiffuseColor(glm::vec3(1.0f));
			_material.setAbsorbtion(0.9f);
		} else if (m == CORNELL_RIGHT) {
			_material.setDiffuseColor(glm::vec3(0.0f, 1.0f, 0.0f));
			_material.setAbsorbtion(0.9f);
		} else if (m == CORNELL_LEFT) {
			_material.setDiffuseColor(glm::vec3(1.0f, 0.0f, 0.0f));
			_material.setAbsorbtion(0.9f);
		}
		
		return;		
	}
	Material getMaterial() const { return _material; }
	void updateAABB() { createAABB(); }
 protected:
	std::string _name;
    AABB *_boundingBox;
	Material _material;
    virtual void createAABB() = 0;
};

#endif
