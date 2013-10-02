#include <iostream>
#include "sphere.h"
#include "pointlight.h"
#include "utils.h"
#include "imageexporter.h"
#include "simpleraycaster.h"
#include "raytracer.h"
#include "whittedtracer.h"
#include "timer.h"
//#include "plane.h"
#include "quadrilateral.h"
int main(int argc, char **argv) {
	bool exportImage = true;
	for (int i = 0; i < argc; ++i) {
		if (std::string(argv[i]) == "-e") {
			exportImage = false;
		}
	}

	Camera *cam = new Camera();
	//Plane *plane = new Plane(glm::vec3(0.f), glm::vec3(1.0f),5.f,5.f);

	std::cout << "Creating AABB!\n";
	AABB bb(glm::vec3(-10.0f), glm::vec3(10.0f));

	Octree *tree = new Octree(&bb);

	Sphere *sphere = new Sphere(0.2f, glm::vec3(-0.05f,-0.2f,1.5f));
	sphere->setMaterial(STONE);

	Sphere *sp2 = new Sphere(1.f, glm::vec3(0.02f, 0.13f, 4.23f));
	sp2->setMaterial(MIRROR);

	Sphere *sp3 = new Sphere(1.f, glm::vec3(-2.02f, 0.13f, 4.23f));
	sp3->setMaterial(STONE);

	Sphere *sp4 = new Sphere(1.f, glm::vec3(-2.32f, -2.f, 4.23f));
	sp4->setMaterial(STONE);
	
	Sphere *spLight = new Sphere(0.1f, glm::vec3(0.0f,-4.0f, 5.0f));
	spLight->setMaterial(LIGHT);

	PointLight *ptLgt = new PointLight(glm::vec3(0.0f, -25.f, 1.0f), 1, glm::vec3(1.f));

	Quadrilateral *quad = new Quadrilateral(
		glm::vec3(-0.5f,1.f,-3.f),
		glm::vec3(0.5f,1.f,-3.f),
		glm::vec3(-0.5f,0.f,-3.f),
		glm::vec3(0.5f,0.f,-3.f));

	quad->setMaterial(MARBLE);


	tree->addObject(sphere);
	tree->addObject(sp2);
	tree->addObject(sp3);
	tree->addObject(sp4);
	tree->addObject(spLight);
	tree->addPointLight(ptLgt);
	tree->addObject(quad);
	tree->print();
	float* pixels = new float[3 * WIDTH * HEIGHT];
	int* pixelsInt = new int[3 * WIDTH * HEIGHT];

	Raytracer rayTracer(WIDTH, HEIGHT);
	SimpleRaycaster caster(WIDTH, HEIGHT);
	WhittedTracer wTracer(WIDTH, HEIGHT);

	wTracer.render(pixels, tree, cam);
// 	caster.render(pixels, tree, cam);
// 	int iters = 1;
// 	rayTracer.render(pixels, tree, cam, iters);


	for(int i=0; i<3*WIDTH*HEIGHT; ++i)
		pixelsInt[i] = int(pixels[i]*255.0f);
	
 	if (exportImage)
		ImageExporter::saveImage(pixelsInt, (char*)"render1", WIDTH, HEIGHT);
	return 0;
}
