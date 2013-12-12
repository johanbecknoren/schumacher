#ifndef __MESH_H__

#include <vector>
#include "triangle.h"

class Mesh{
public:
	
	Mesh();
	std::vector<Triangle*> *createTriangles(std::string objName);
	~Mesh();
	void printBbox();
	void updateBbox();
	Vec3 getCenter();
private:
	std::vector<Vec3> verts;
	std::vector<Triangle *> *triangles;
	AABB _bbox;
	
};

#endif
