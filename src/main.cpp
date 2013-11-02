#include <iostream>
#include "sphere.h"
#include "pointlight.h"
#include "utils.h"
#include "imageexporter.h"
#include "simpleraycaster.h"
#include "whittedtracer.h"
#include "timer.h"
#include "glrenderer.h"
#include "quadrilateral.h"
#include "cornellboxfactory.h"
#include "triangle.h"
#include "montecarlotracer.h"
#include "scenebuilder.h"

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

	Sphere *sphere = new Sphere(0.2f, glm::vec3(-0.05f,-0.2f,0.0f));
	sphere->setMaterial(MARBLE);

	Sphere *sp2 = new Sphere(1.f, glm::vec3(1.02f, 0.13f, 4.23f));
	sp2->setMaterial(MIRROR);

	Sphere *sp3 = new Sphere(1.f, glm::vec3(-2.02f, 0.13f, 4.23f));
	sp3->setMaterial(STONE);

	Sphere *sp4 = new Sphere(1.f, glm::vec3(-2.32f, -2.f, 4.23f));
	sp4->setMaterial(MARBLE);
	
	Sphere *spLight = new Sphere(0.1f, glm::vec3(0.0f,-4.0f, 5.0f));
	spLight->setMaterial(LIGHT);
	
	Triangle *tri = new Triangle(
			glm::vec3(10.f, 10.f, 30.f),
			glm::vec3(-10.f, -10.f, 30.f),
			glm::vec3(10.f, -10.f, 30.f));
	tree->addObject(tri);


	PointLight *ptLgt = new PointLight(glm::vec3(0.0f, 1.f, -2.0f), 1, glm::vec3(0.3f));
// 	tree->addPointLight(ptLgt);
	PointLight *l2 = new PointLight(glm::vec3(5.0f, 1.4f, -0.4f), 1, glm::vec3(0.3f));
	tree->addPointLight(l2);
// 	Quadrilateral *quad = new Quadrilateral(
// 				  glm::vec3( 1., 1., 5.),
//                   glm::vec3( 1., -1., 5.),
//                   glm::vec3(-1., -1., 5.),
// 				  glm::vec3( -1., 1., 5.));
// 	quad->setMaterial(CORNELL_LEFT);


	std::vector<Renderable *> scene = SceneBuilder::createCornellBox();

	tree->addObject(scene);
/*	tree->addObject(sphere);
	tree->addObject(sp2);
	tree->addObject(sp3);
	tree->addObject(sp4);*/

// 	tree->addObject(spLight);
	
//	tree->addObject(quad);
	tree->print();
	float* pixels = new float[3 * WIDTH * HEIGHT];
	int* pixelsInt = new int[3 * WIDTH * HEIGHT];
#ifdef USE_OPENGL
	glfwInit();
	
	// GLFW_WINDOW
	if(!glfwOpenWindow(WIDTH, HEIGHT, 8,8,8,8, 32,0, GLFW_WINDOW)) {
		glfwTerminate(); // glfwOpenWindow failed, quit the program.
		return 0;
	}
	
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);        // making default background color black
    glfwSwapInterval(0);
#endif
	
	SimpleRaycaster caster(WIDTH, HEIGHT);
	WhittedTracer wTracer(WIDTH, HEIGHT);
	MonteCarloRayTracer mTracer(WIDTH, HEIGHT);

	mTracer.render(pixels, tree, cam);
//	wTracer.render(pixels, tree, cam);

	// caster.render(pixels, tree, cam);
// 	int iters = 1;	
	

	for(int i=0; i<3*WIDTH*HEIGHT; ++i)
		pixelsInt[i] = int(pixels[i]*255.0f);

	if (exportImage)
		ImageExporter::saveImage(pixelsInt, (char*)"render1", WIDTH, HEIGHT);
	
	bool quitProgram = false;
#ifdef USE_OPENGL
	while(!quitProgram) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawPixels(WIDTH,HEIGHT,GL_RGB,GL_FLOAT,pixels);
		glfwSwapBuffers();

		if(glfwGetKey('Q') || !glfwGetWindowParam(GLFW_OPENED)) {
			quitProgram = true;
		}
	}
	glfwTerminate();
#endif


	delete sphere;
	delete sp2;
	delete sp3;
	delete sp4;
	delete tree;
	delete pixels;

	SceneBuilder::destructCornellBox(scene);
	return 0;
}
