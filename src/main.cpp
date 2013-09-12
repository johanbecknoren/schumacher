#include <iostream>
#include "renderable.h"
#include "AABB.h"

int main() {

	std::cout << "Creating AABB!\n";
	AABB bb(glm::vec3(-1.0f), glm::vec3(1.0f), glm::vec3(0.0f));

	return 0;
}
