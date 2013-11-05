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
#include "materialmanager.h"

int main(int argc, char **argv) {
	bool exportImage = true;
    bool singleThread = false;
    bool renderDuring = true;
	for (int i = 0; i < argc; ++i) {
		if (std::string(argv[i]) == "-e") {
			exportImage = false;
		}
        else if (std::string(argv[i]) == "-s") {
            singleThread = true;
        }
        else if (std::string(argv[i]) == "-r") {
            renderDuring = false;
        }
	}
	
	Camera *cam = new Camera();

	AABB bb(glm::vec3(-10.0f), glm::vec3(10.0f));

	Octree *tree = new Octree(&bb);

	MaterialManager mm = MaterialManager();

	PointLight *l2 = new PointLight(glm::vec3(5.0f, 1.4f, -0.4f), 1, glm::vec3(0.3f));
	tree->addPointLight(l2);

	std::vector<Renderable *> scene = SceneBuilder::createCornellBox();
	tree->addObject(scene);

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
	
//	SimpleRaycaster caster(WIDTH, HEIGHT);
//	WhittedTracer wTracer(WIDTH, HEIGHT);

	MonteCarloRayTracer mTracer(WIDTH, HEIGHT);

    mTracer.render(pixels, tree, cam, singleThread, renderDuring);
//	wTracer.render(pixels, tree, cam);

	// caster.render(pixels, tree, cam);	

	for(int i = 0; i < 3 * WIDTH * HEIGHT; ++i)
		pixelsInt[i] = int(pixels[i] * 255.0f);

	if (exportImage)
		ImageExporter::saveImage(pixelsInt, (char*)"render1", WIDTH, HEIGHT);
	
	bool quitProgram = false;
#ifdef USE_OPENGL
	while(!quitProgram) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_FLOAT, pixels);
        glfwSwapBuffers();

        if(glfwGetKey('Q') || !glfwGetWindowParam(GLFW_OPENED)) {
			quitProgram = true;
		}
	}
	glfwTerminate();
#endif

	delete tree;
	delete pixels;
    delete pixelsInt;
	//SceneBuilder::destructCornellBox(scene);
	return 0;
}
