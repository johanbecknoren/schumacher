#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "AABB.h"
#include "intersectionpoint.h"
#include "utils.h"
#include "materialmanager.h"
#include <memory>

class Renderable {
 public:
    virtual ~Renderable() { };
    AABB *getBoundingBox() { return _boundingBox.get(); };
    virtual bool getIntersectionPoint(Ray &ray,  IntersectionPoint &ip) const = 0;
	virtual std::string getName() const { return "No name"; };
	virtual std::string asString() const { return "asString() not implemented for " + getName(); };
	virtual void scale(const float s) {
		std::cout << "Using unimplemented scale in " << getName() << std::endl;
	}
	virtual void translate(const glm::vec3 &t) {
		std::cout << "Using unimplemented translate in " << getName() << std::endl;

	}
	void setMaterial(const material_t& m) {
		_material = MaterialManager::get(m);
		return;
	}

	Material* getMaterial() const { return _material; }
	void updateAABB() { createAABB(); }
 protected:
	std::shared_ptr<AABB> _boundingBox;
	Material *_material;
    virtual void createAABB() = 0;
};

#endif
