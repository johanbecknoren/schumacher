#ifndef __MESH_H__

#include <vector>
#include "triangle.h"

class Mesh{
public:
	
	Mesh();
	std::vector<Triangle*> *createTriangles(std::string objName);
	~Mesh();
private:
	std::vector<glm::vec3> verts;
	std::vector<Triangle> triangles;
};

#endif
