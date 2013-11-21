#include "mesh.h"

#include <tinyobjloader/tiny_obj_loader.h>

#include <fstream>

#include "glmprint.h"

Mesh::Mesh() {
	
}

std::vector<Triangle> Mesh::createTriangles(std::string objName) {
	triangles = std::vector<Triangle>();
	verts = std::vector<glm::vec3>();

    std::vector<tinyobj::shape_t> shapes;
    std::string err = tinyobj::LoadObj(shapes, objName.c_str());
    if (!err.empty()) {
        std::cerr << err << std::endl;
        exit(1);
    }

    std::cout << shapes.size() << " meshes found\n";
    for (unsigned int n = 0, vertOffset = 0; n < shapes.size(); ++n) {
        
        unsigned int numFaces = shapes[n].mesh.indices.size() / 3;
        unsigned int numVerts = shapes[n].mesh.positions.size() / 3;
		std::cout << numFaces << " f, " << numVerts << " v\n";
		for (unsigned int i = 0; i < numVerts; ++i) {
            verts.push_back(glm::vec3(shapes[n].mesh.positions[i * 3],
									  shapes[n].mesh.positions[i * 3 + 1],
									  shapes[n].mesh.positions[i * 3 + 2]));
			
		}
		std::cout << verts.size() << " verts loaded.\n";

        for (unsigned int i = 0; i < numFaces; ++i) {
			std::cout << "Creating face " << i << std::endl;
			
			std::vector<unsigned int> val = std::vector<unsigned int>(3);
			for (unsigned int u = 0; u < 3; ++u) {
               val[u] = shapes[n].mesh.indices[i * 3 + u + vertOffset];
			}

			std::cout  <<" " << val[0] <<" " << val[1] << " " << val[2] <<" " <<
				  " created\n";
			
			glm::vec3 v0 = verts[val[0]];
			glm::vec3 v1 = verts[val[1]];
			glm::vec3 v2 = verts[val[2]];
			
			std::cout << v0 << v1 << v2;
			Triangle t = Triangle(v0, v1, v2);
			triangles.push_back(t);
			std::cout << "dune\n";
		}
		std::cout << "Mesh " << n << " loaded\n";
		vertOffset += numVerts;
	}
	return triangles;
}
