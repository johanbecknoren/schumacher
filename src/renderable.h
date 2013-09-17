#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "AABB.h"
#include "intersectionpoint.h"
#include "utils.h"

class Renderable {
 public:
    virtual ~Renderable() { delete _boundingBox; };
    AABB *getBoundingBox() { return _boundingBox; };
    virtual IntersectionPoint *getIntersectionPoint(Ray *ray) const = 0;
	std::string getName() const { return _name; };
	virtual std::string asString() const { return "asString() not implemented for " + getName(); };
	void setMaterial(const material_t& m) {
		_material = Material();
		if(m == STONE)
			return;
		else if (m == GLASS) {
			_material.setDiffuseColor(glm::vec3(1.0f));
			_material.setRefractionIndex(REFRACTION_GLASS);
			_material.setOpacity(0.03);
			_material.setSpecular(1.0f);
		} else if (m == MARBLE) {
			_material.setDiffuseColor(glm::vec3(1.0f, 0.0f,0.0f));
			_material.setSpecular(0.8f);
		}
		
	}
 protected:
	std::string _name;
    AABB *_boundingBox;
	Material _material;
    virtual void createAABB() = 0;
};

#endif
