#ifndef CORNELLBOXFACTORY_H
#define CORNELLBOXFACTORY_H

#include "quadrilateral.h"



class CornellBoxFactory {

	public: 
		static Quadrilateral createFloor() {
		Quadrilateral f = Quadrilateral(
			glm::vec3(552.8f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f), 			
			glm::vec3(0.0f, 0.0f, 559.2f), 		
			glm::vec3(549.6f, 0.0f, 559.2f));
		
		f.scale(scaleFactor);
		return f;

	}

	static Quadrilateral createCeil() {
		Quadrilateral c = Quadrilateral(
			glm::vec3(556.0f,   548.8f, 0.0f),
			glm::vec3(556.0f,   548.8f, 559.2f), 			
			glm::vec3(0.0f,     548.8f, 559.2f), 		
			glm::vec3(0.0f,     548.8f, 0.0f));
		c.scale(scaleFactor);
		return c;

	}

	static Quadrilateral createBack() {
		Quadrilateral b = Quadrilateral(
			glm::vec3(549.6, 0.0, 559.2),
			glm::vec3(0.0,   0.0, 559.2), 			
			glm::vec3(0.0, 548.8, 559.2), 		
			glm::vec3(556.0, 548.8, 559.2));
		b.scale(scaleFactor);
		return b;

	}
	private:
		static float scaleFactor;
};
float CornellBoxFactory::scaleFactor = 0.1f;
#endif
