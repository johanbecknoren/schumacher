#ifndef SCENEBUILDER_H
#define SCENEBUILDER_H

#include <vector>
#include "cornellboxfactory.h"
#include "quadrilateral.h"
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
		scene.push_back(boxFront);
		
		createTallBox(scene, LIGHT);
		createShortBox(scene, GLASS);

		return scene;
	}
	static void destructCornellBox(std::vector<Renderable *> r) {
		for (size_t i = 0; i < r.size(); ++i) {
			delete r[i];
		}
		
	}

};

#endif
