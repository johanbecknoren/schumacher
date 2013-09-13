#include <iostream>
#include "octree.h"
#include "sphere.h"

int main() {
	std::cout << "Creating AABB!\n";
	AABB bb(glm::vec3(-10.0f), glm::vec3(10.0f));

	Octree tree(&bb);
	Sphere *sphere = new Sphere(1.0f, glm::vec3(0.0f));
	tree.addObject(sphere);
	tree.print();
	return 0;
}
