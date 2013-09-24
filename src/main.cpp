#include <iostream>
#include "sphere.h"
#include "lightsource.h"
#include "utils.h"
#include "imageexporter.h"
#include "raytracer.h"
#include "timer.h"

int main() {
	Camera *cam = new Camera();

	Ray *r = new Ray(glm::vec3(-15.f,-15.f,-15.f), glm::vec3(1.0,0.7f,0.8f));
	glm::vec3 point(-9.5f,9.5f,9.5f);
	Ray *r2 = new Ray(glm::vec3(0.0f), glm::vec3(-1.0f));
	Ray *r3 = new Ray(glm::vec3(-15.f), glm::vec3(1.0f));
	std::cout << "Creating AABB!\n";
	AABB bb(glm::vec3(-10.0f), glm::vec3(10.0f));

	Octree *tree = new Octree(&bb);

	Sphere *sphere = new Sphere(0.2f, glm::vec3(-0.05f,-0.2f,1.5f));
	sphere->setMaterial(STONE);

	Sphere *sp2 = new Sphere(1.f, glm::vec3(0.02f, 0.13f, 4.23f));
	sp2->setMaterial(GLASS);

	Sphere *sp3 = new Sphere(1.f, glm::vec3(-2.02f, 0.13f, 4.23f));
	sp3->setMaterial(MARBLE);

	Sphere *spLight = new Sphere(0.1f, glm::vec3(0.0f,-4.0f, 5.0f));
	spLight->setMaterial(LIGHT);

	tree->addObject(sphere);
	tree->addObject(sp2);
//	tree->addObject(sphere);
	tree->addObject(sp3);
	tree->addObject(spLight);

	float* pixels = new float[3 * WIDTH * HEIGHT];
	int* pixelsInt = new int[3 * WIDTH * HEIGHT];
	Timer::getInstance()->start("tracing");	

	Raytracer rayTracer;
	int iters = 1;
	rayTracer.render(pixels, tree, WIDTH, HEIGHT, cam, iters);

	Timer::getInstance()->stop("tracing");
	Timer::getInstance()->printRealTime("tracing");

	Timer::getInstance()->start("tracing");
	for(int i=0; i<3*WIDTH*HEIGHT; ++i)
		pixelsInt[i] = int(pixels[i]*255.0f);
	

	ImageExporter::saveImage(pixelsInt, (char*)"render1", WIDTH, HEIGHT);
	Timer::getInstance()->stop("tracing");
	Timer::getInstance()->printRealTime("tracing");
	return 0;
}
