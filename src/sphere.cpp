#include "sphere.h"

Sphere::Sphere(const float &radius, const glm::vec3 &position) {
    _radius = radius;
    _position = position;
    createAABB();
}

IntersectionPoint *Sphere::getIntersectionPoint(Ray *ray) const {
        return new IntersectionPoint();
}

void Sphere::createAABB() {
    boundingBox = new AABB(glm::vec3(-10.0f), glm::vec3(10.0f), glm::vec3(0.0f));
}
