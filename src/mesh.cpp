#include "mesh.h"
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <fstream>

#include "glmprint.h"

Mesh::Mesh() {
	
}

std::vector<Triangle> Mesh::createTriangles(std::string objName) const {
	std::vector<Triangle> triangles = std::vector<Triangle>();
	std::vector<glm::vec3> verts = std::vector<glm::vec3>();
	Assimp::Importer importer = Assimp::Importer();
	std::ifstream fin(objName.c_str());
	if (!fin.fail()) {
		fin.close();
	}
	else {
		std::cout << "Couldn\'t find file " << objName << std::endl;
		std::cout << importer.GetErrorString() << std::endl;
		return triangles;
	}

	const aiScene *scene = importer.ReadFile(objName.c_str(), aiProcessPreset_TargetRealtime_Quality);
	if (!scene) {
		std::cout << "Error: " << importer.GetErrorString() << std::endl;
		return triangles;
	}
	std::cout << scene->mNumMeshes << " meshes found\n";
	for (unsigned int n = 0, vertOffset = 0; n < scene->mNumMeshes; ++n) {
		const aiMesh *mesh = scene->mMeshes[n];
		unsigned int numFaces = mesh->mNumFaces;
		unsigned int numVerts = mesh->mNumVertices;
		std::cout << numFaces << " f, " << numVerts << " v\n";
		for (unsigned int i = 0; i < numVerts; ++i) {
			verts.push_back(glm::vec3(mesh->mVertices[i].x,
									  mesh->mVertices[i].y,
									  mesh->mVertices[i].z));
			
		}
		std::cout << verts.size() << " verts loaded.\n";

		for (unsigned int i = 0; i < numFaces; ++i) {
			std::cout << "Creating face " << i << std::endl;
			const struct aiFace *face = &mesh->mFaces[i];
			std::vector<unsigned int> val = std::vector<unsigned int>(3);
			for (unsigned int u = 0; u < 3; ++u) {
				val[u] = face->mIndices[u] + vertOffset;
			}

			std::cout  <<" " << val[0] <<" " << val[1] << " " << val[2] <<" " <<
				  " created\n";
			
			glm::vec3 *v0 = &verts[val[0]];
			glm::vec3 *v1 = &verts[val[1]];
			glm::vec3 *v2 = &verts[val[2]];
			if (v0 == NULL) {
				std::cout << "v0 is null\n";
				continue;
			}
			if (v1 == NULL) {
				std::cout << "v1 is null\n";
				continue;
			}
			if (v2 == NULL) {
				std::cout << "v2 is null\n";
				continue;
			}
			
			Triangle *t = new Triangle(v0, v1, v2);
			std::cout << *v0 << *v1 << *v2;
			triangles.push_back(*t);
			std::cout << t->getX1().x << "dune\n";
		}
		std::cout << "Mesh " << n << " loaded\n";
		vertOffset += numVerts;
	}
	
	return triangles;
}
