#ifndef SCENEBUILDER_H
#define SCENEBUILDER_H

#include <vector>
#include "cornellboxfactory.h"
#include "quadrilateral.h"
#include "mesh.h"

class SceneBuilder {

private:
	static void createShortBox(std::vector<Renderable *> &scene, material_t m) {
		Quadrilateral *shortBlock1 = new Quadrilateral(CornellBoxFactory::createShortBlock1());
		shortBlock1->setMaterial(m);
		scene.push_back(shortBlock1);

		Quadrilateral *shortBlock2 = new Quadrilateral(CornellBoxFactory::createShortBlock2());
		shortBlock2->setMaterial(m);
		scene.push_back(shortBlock2);

		Quadrilateral *shortBlock3 = new Quadrilateral(CornellBoxFactory::createShortBlock3());
		shortBlock3->setMaterial(m);
		scene.push_back(shortBlock3);

		Quadrilateral *shortBlock4 = new Quadrilateral(CornellBoxFactory::createShortBlock4());
		shortBlock4->setMaterial(m);
		scene.push_back(shortBlock4);

		Quadrilateral *shortBlock5 = new Quadrilateral(CornellBoxFactory::createShortBlock5());
		shortBlock5->setMaterial(m);
		scene.push_back(shortBlock5);
	}

	static void createTallBox(std::vector<Renderable *> &scene, material_t m) {
		Quadrilateral *tallBlock1 = new Quadrilateral(CornellBoxFactory::createTallBlock1());
		tallBlock1->setMaterial(m);
		scene.push_back(tallBlock1);

		Quadrilateral *tallBlock2 = new Quadrilateral(CornellBoxFactory::createTallBlock2());
		tallBlock2->setMaterial(m);
		scene.push_back(tallBlock2);

		Quadrilateral *tallBlock3 = new Quadrilateral(CornellBoxFactory::createTallBlock3());
		tallBlock3->setMaterial(m);
		scene.push_back(tallBlock3);

		Quadrilateral *tallBlock4 = new Quadrilateral(CornellBoxFactory::createTallBlock4());
		tallBlock4->setMaterial(m);
		scene.push_back(tallBlock4);

		Quadrilateral *tallBlock5 = new Quadrilateral(CornellBoxFactory::createTallBlock5());
		tallBlock5->setMaterial(m);
		scene.push_back(tallBlock5);
	}
	
	static void createLightSourceQuad(std::vector<Renderable *> &scene, material_t m) {
		Quadrilateral *lightQuad = new Quadrilateral(CornellBoxFactory::createLightSourceQuad());
		lightQuad->setMaterial(m);
		scene.push_back(lightQuad);
	}

	static void createBunny(std::vector<Renderable *> &scene, material_t m) {
		Mesh *mesh = new Mesh();
		std::vector<Triangle *> *triangles =
			mesh->createTriangles("../models/bunny_small.obj");
		
		for (unsigned int s = 0; s < triangles->size(); ++s) {
			triangles->at(s)->setMaterial(m);
			
			Triangle *tri = (*triangles)[s];
			tri->scale(0.2);
			// tri->translate(glm::vec3(.0, 0.01, 10));

			// triangles->at(s)->scale(0.2);

			scene.push_back(tri);
		}
		mesh->updateBbox();
		mesh->printBbox();
	}
	
public:	
	
	static std::vector<Renderable *> createCornellBox() {
		std::vector<Renderable *> scene;
		Quadrilateral *boxCeiling = new Quadrilateral(CornellBoxFactory::createCeil());
		boxCeiling->setMaterial(CORNELL_CEIL);
		scene.push_back(boxCeiling);
		
		Quadrilateral *boxBack = new  Quadrilateral(CornellBoxFactory::createBack());
		boxBack->setMaterial(CORNELL_BACK);
		scene.push_back(boxBack);

		Quadrilateral *boxFloor = new Quadrilateral(CornellBoxFactory::createFloor());
		boxFloor->setMaterial(CORNELL_FLOOR);
		scene.push_back(boxFloor);
		
		Quadrilateral *boxRight = new Quadrilateral(CornellBoxFactory::createRight());
		boxRight->setMaterial(CORNELL_RIGHT);
		scene.push_back(boxRight);
		
		Quadrilateral *boxLeft = new Quadrilateral(CornellBoxFactory::createLeft());
		boxLeft->setMaterial(CORNELL_LEFT);
		scene.push_back(boxLeft);

		Quadrilateral *boxFront = new Quadrilateral(CornellBoxFactory::createFront());
		boxFront->setMaterial(CORNELL_CEIL);
		// scene.push_back(boxFront);

		Sphere* sp_glass = new Sphere(1.5f, glm::vec3(3.f,-5.5f,17.5f) );
		sp_glass->setMaterial(GLASS);
		scene.push_back(sp_glass);

		Sphere* sp_mirror = new Sphere(1.5f, glm::vec3(-3.f,0.f,22.0f) );
		sp_mirror->setMaterial(MIRROR);
		scene.push_back(sp_mirror);

		createTallBox(scene, LIGHT);
		createShortBox(scene, CORNELL_CEIL);
		createLightSourceQuad(scene, LIGHT);
		createBunny(scene, CORNELL_LEFT);
		// Triangle * t = new Triangle(glm::vec3(-1, -1, 2), glm::vec3(-1, 1, 1),
									// glm::vec3(1, 1, 1));
		// t->setMaterial(CORNELL_LEFT);
		// scene.push_back(t);
		return scene;
	}

	
	
	static void destructCornellBox(std::vector<Renderable *> r) {
		for (size_t i = 0; i < r.size(); ++i) {
            if (r[i] != NULL)
                delete r[i];
		}
		
	}

};

#endif
