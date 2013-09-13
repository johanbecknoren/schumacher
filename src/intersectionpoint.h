#ifndef INTERSECTIONPOINT_H
#define INTERSECTIONPOINT_H

class IntersectionPoint {
public:
    IntersectionPoint() {};
	IntersectionPoint(const glm::vec3& p, const glm::vec3& n) {
		_point = p;
		_surfaceNormal = n;
	};

	glm::vec3 getPoint() const { return _point; }
	glm::vec3 getNormal() const { return _surfaceNormal; }
private:
    glm::vec3 _point;
    glm::vec3 _surfaceNormal;
    // Material material;
    // Outgoing ray?
};

#endif
