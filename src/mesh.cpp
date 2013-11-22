#include "mesh.h"

#include <tinyobjloader/tiny_obj_loader.h>

#include <fstream>

#include "glmprint.h"

Mesh::Mesh() {
	
}

void Mesh::updateBbox() {
	glm::vec3 minv = verts[0];
	glm::vec3 maxv = verts[0];
	for (unsigned int i = 0; i < verts.size(); ++i) {
		minv.x = glm::min(verts[i].x, minv.x);
		minv.y = glm::min(verts[i].y, minv.y);
		minv.z = glm::min(verts[i].z, minv.z);

		maxv.x = glm::max(verts[i].x, maxv.x);
		maxv.y = glm::max(verts[i].y, maxv.y);
		maxv.z = glm::max(verts[i].z, maxv.z);
	}
	_bbox = AABB(minv, maxv);
}

void Mesh::printBbox() {
	_bbox.print();
}

std::vector<Triangle*> *Mesh::createTriangles(std::string objName) {
	triangles = new std::vector<Triangle*>();
	verts = std::vector<glm::vec3>();

    std::vector<tinyobj::shape_t> shapes;
    std::string err = tinyobj::LoadObj(shapes, objName.c_str());
    if (!err.empty()) {
        std::cerr << err << std::endl;
        return triangles;
    }

    for (unsigned int n = 0, vertOffset = 0; n < shapes.size(); ++n) {
        
        unsigned int numFaces = shapes[n].mesh.indices.size() / 3;
        unsigned int numVerts = shapes[n].mesh.positions.size() / 3;

		for (unsigned int i = 0; i < numVerts; ++i) {
            verts.push_back(glm::vec3(shapes[n].mesh.positions[i * 3],
									  shapes[n].mesh.positions[i * 3 + 1],
									  shapes[n].mesh.positions[i * 3 + 2]));
			
		}


        for (unsigned int i = 0; i < numFaces; ++i) {
			
			std::vector<unsigned int> val = std::vector<unsigned int>(3);
			for (unsigned int u = 0; u < 3; ++u) {
               val[u] = shapes[n].mesh.indices[i * 3 + u + vertOffset];
			}

			glm::vec3 v0 = verts[val[0]];
			glm::vec3 v1 = verts[val[1]];
			glm::vec3 v2 = verts[val[2]];
			
			Triangle *t = new Triangle(v0, v1, v2);
			triangles->push_back(t);
		}
		vertOffset += numVerts;
	}


	
	return triangles;
}
