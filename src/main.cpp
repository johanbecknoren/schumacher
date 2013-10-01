#include <iostream>
#include "sphere.h"
#include "pointlight.h"
#include "utils.h"
#include "imageexporter.h"
#include "simpleraycaster.h"
#include "raytracer.h"
#include "timer.h"
#include "plane.h"
int main(int argc, char **argv) {
	bool exportImage = true;
	for (int i = 0; i < argc; ++i) {
		if (std::string(argv[i]) == "-e") {
			exportImage = false;
		}
	}

	Camera *cam = new Camera();
	Plane *plane = new Plane(glm::vec3(0.f), glm::vec3(1.0f),5.f,5.f);

	std::cout << "Creating AABB!\n";
	AABB bb(glm::vec3(-10.0f), glm::vec3(10.0f));

	Octree *tree = new Octree(&bb);

	Sphere *sphere = new Sphere(0.2f, glm::vec3(-0.05f,-0.2f,1.5f));
	sphere->setMaterial(STONE);

	Sphere *sp2 = new Sphere(1.f, glm::vec3(0.02f, 0.13f, 4.23f));
	sp2->setMaterial(GLASS);

	Sphere *sp3 = new Sphere(1.f, glm::vec3(-2.02f, 3.13f, 4.23f));
	sp3->setMaterial(MARBLE);

	Sphere *spLight = new Sphere(0.1f, glm::vec3(0.0f,-4.0f, 5.0f));
	spLight->setMaterial(LIGHT);

	tree->addObject(sphere);
	tree->addObject(sp2);
	tree->addObject(sp3);
	tree->addObject(spLight);

	float* pixels = new float[3 * WIDTH * HEIGHT];
	int* pixelsInt = new int[3 * WIDTH * HEIGHT];

	Raytracer rayTracer;
	SimpleRaycaster caster;
	caster.render(pixels, tree, WIDTH, HEIGHT, cam);
// 	int iters = 1;
// 	rayTracer.render(pixels, tree, WIDTH, HEIGHT, cam, iters);


	for(int i=0; i<3*WIDTH*HEIGHT; ++i)
		pixelsInt[i] = int(pixels[i]*255.0f);
	
 	if (exportImage)
		ImageExporter::saveImage(pixelsInt, (char*)"render1", WIDTH, HEIGHT);
	return 0;
}
