#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "renderable.h"

class Triangle : public Renderable {
	public:
		Triangle(glm::vec3 x0, glm::vec3 x1, glm::vec3 x2);

		IntersectionPoint *getIntersectionPoint(Ray *ray) const;
		virtual void createAABB();
		
	private:
		glm::vec3 x1;
		glm::vec3 x2;
		glm::vec3 x0;
		glm::vec3 normal;
};

#endif
