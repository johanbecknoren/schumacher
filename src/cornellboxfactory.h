#ifndef CORNELLBOXFACTORY_H
#define CORNELLBOXFACTORY_H

#include "quadrilateral.h"
#include "triangle.h"


class CornellBoxFactory {

	public:
		static Quadrilateral createFloor() {
		Quadrilateral f = Quadrilateral(

			Vec3(real(552.8),	real(0.0), real(0.0)),
			Vec3(real(0.0),		real(0.0), real(0.0)), 			
			Vec3(real(0.0),		real(0.0), real(559.2)), 		
			Vec3(real(549.6),	real(0.0), real(559.2)));
		
		f.scale(_scaleFactor);
		f.translate(_t);
		f.updateAABB();
		return f;
	}

	static Quadrilateral createFront() {
		Quadrilateral f = Quadrilateral(Vec3(real(0.),		real(548.8),	real(0.)),
										Vec3(real(0.),		real(0.),		real(0.)),
										Vec3(real(552.8),	real(0.),		real(0.)),
										Vec3(real(556.),	real(548.8),	real(0.)));
		f.scale(_scaleFactor);
		f.translate(_t);
		f.updateAABB();
		return f;
	}
	
	static Quadrilateral createCeil() {
		Quadrilateral c = Quadrilateral(

			Vec3(real(556.0),   real(548.8), real(0.0)),
			Vec3(real(556.0),   real(548.8), real(559.2)), 			
			Vec3(real(0.0),     real(548.8), real(559.2)), 		
			Vec3(real(0.0),     real(548.8), real(0.0f)));
		c.scale(_scaleFactor);
		c.translate(_t);
		c.updateAABB();
		return c;
	}

	static Quadrilateral createBack() {
		Quadrilateral b = Quadrilateral(

			Vec3(real(549.6), 	real(0.0), 	real(559.2)),
			Vec3(real(0.0),   	real(0.0), 	real(559.2)),
			Vec3(real(0.0), 	real(548.8), real(559.2)), 		
			Vec3(real(556.0), 	real(548.8), real(559.2)));
		b.scale(_scaleFactor);
		b.translate(_t);
		b.updateAABB();
		return b;
	}

	static Quadrilateral createRight() {
		Quadrilateral r = Quadrilateral(

			Vec3(real(0.0), real(0.0),	real(559.2)),
			Vec3(real(0.0), real(0.0),	real(0.0)), 			
			Vec3(real(0.0), real(548.8),	real(0.0)), 		
			Vec3(real(0.0), real(548.8),	real(559.2)));
			r.scale(_scaleFactor);
		r.translate(_t);
		r.updateAABB();
		return r;
	}
	static Quadrilateral createLeft() {
		Quadrilateral l = Quadrilateral(

			Vec3(real(552.8), real(0.0),	real(0.0)),
			Vec3(real(549.6), real(0.0),	real(559.2)), 			
			Vec3(real(556.0), real(548.8),	real(559.2)), 		
			Vec3(real(556.0), real(548.8),	real(0.0)));
		l.scale(_scaleFactor);
		l.translate(_t);
		l.updateAABB();
		return l;
	}
	//Forts'tt h'r!!!
	static Quadrilateral createShortBlock1() {
		Quadrilateral sb1 = Quadrilateral(
			Vec3(real(130.0),	real(165.0),	real(65.0)),
			Vec3(real(82.0),	real(165.0),	real(225.0)),
			Vec3(real(240.0),	real(165.0),	real(272.0)),
			Vec3(real(290.0),	real(165.0),	real(114.0)));
		sb1.scale(_scaleFactor);
		sb1.translate(_t);
		sb1.updateAABB();
		return sb1;
	}
	static Quadrilateral createShortBlock2() {
		Quadrilateral sb2 = Quadrilateral(
			Vec3(real(290.0),	real(0.0),		real(114.0)),
			Vec3(real(290.0),	real(165.0),	real(114.0)),
			Vec3(real(240.0),	real(165.0),	real(272.0)),
			Vec3(real(240.0),	real(0.0),		real(272.0)));
		sb2.scale(_scaleFactor);
		sb2.translate(_t);
		sb2.updateAABB();
		return sb2;
	}
	static Quadrilateral createShortBlock3() {
		Quadrilateral sb3 = Quadrilateral(
			Vec3(real(130.0),	real(0.00),		real(65.0)),
			Vec3(real(130.0),	real(165.0),	real(65.0)),
			Vec3(real(290.0),	real(165.0),	real(114.0)),
			Vec3(real(290.0),	real(0.0),		real(114.0)));
		sb3.scale(_scaleFactor);
		sb3.translate(_t);
		sb3.updateAABB();
		return sb3;
	}
	static Quadrilateral createShortBlock4() {
		Quadrilateral sb4 = Quadrilateral(
			Vec3(real(82.0),	real(0.0),		real(225.0)),
			Vec3(real(82.0),	real(165.0),	real(225.0)),
			Vec3(real(130.0),	real(165.0),	real(65.0)),
			Vec3(real(130.0),	real(0.0),		real(65.0)));
		sb4.scale(_scaleFactor);
		sb4.translate(_t);
		sb4.updateAABB();
		return sb4;
	}
	static Quadrilateral createShortBlock5() {
		Quadrilateral sb5 = Quadrilateral(
			Vec3(real(240.0),	real(0.0),	real(272.0)),
			Vec3(real(240.0),	real(165.0),	real(272.0)),
			Vec3(real(82.0),	real(165.0),	real(225.0)),
			Vec3(real(82.0),	real(0.0),	real(225.0)));
		sb5.scale(_scaleFactor);
		sb5.translate(_t);
		sb5.updateAABB();
		return sb5;
	}

	static Quadrilateral createTallBlock1() {
		Quadrilateral tb1 = Quadrilateral(
			Vec3(real(423.0),	real(330.0),	real(247.0)),
			Vec3(real(265.0),	real(330.0),	real(296.0)),
			Vec3(real(314.0),	real(330.0),	real(456.0)),
			Vec3(real(472.0),	real(330.0),	real(406.0)));
		tb1.scale(_scaleFactor);
		tb1.translate(_t);
		tb1.updateAABB();
		return tb1;
	}

	static Quadrilateral createTallBlock2() {
		Quadrilateral tb2 = Quadrilateral(
			Vec3(real(423.0),	real(0.0),	real(247.0)),
			Vec3(real(423.0),	real(330.0),	real(247.0)),
			Vec3(real(472.0),	real(330.0),	real(406.0)),
			Vec3(real(472.0),	real(0.0),	real(406.0)));
		tb2.scale(_scaleFactor);
		tb2.translate(_t);
		tb2.updateAABB();
		return tb2;
	}

	static Quadrilateral createTallBlock3() {
		Quadrilateral tb3 = Quadrilateral(
			Vec3(real(472.0),	real(0.0),	real(406.0)),
			Vec3(real(472.0),	real(330.0),	real(406.0)),
			Vec3(real(314.0),	real(330.0),	real(456.0)),
			Vec3(real(314.0),	real(0.0),	real(456.0)));
		tb3.scale(_scaleFactor);
		tb3.translate(_t);
		tb3.updateAABB();
		return tb3;
	}

	static Quadrilateral createTallBlock4() {
		Quadrilateral tb4 = Quadrilateral(
			Vec3(real(314.0),	real(0.0),		real(456.0)),
			Vec3(real(314.0),	real(330.0),	real(456.0)),
			Vec3(real(265.0),	real(330.0),	real(296.0)),
			Vec3(real(265.0),	real(0.0),		real(296.0)));
		tb4.scale(_scaleFactor);
		tb4.translate(_t);
		tb4.updateAABB();
		return tb4;
	}

	static Quadrilateral createTallBlock5() {
		Quadrilateral tb5 = Quadrilateral(
			Vec3(real(265.0),	real(0.0),		real(296.0)),
			Vec3(real(265.0),	real(330.0),	real(296.0)),
			Vec3(real(423.0),	real(330.0),	real(247.0)),
			Vec3(real(423.0),	real(0.0),		real(247.0)));
		tb5.scale(_scaleFactor);
		tb5.translate(_t);
		tb5.updateAABB();
		return tb5;
	}

	static Quadrilateral createLightSourceQuad() {
		Quadrilateral ls = Quadrilateral(
			Vec3(real(343.0), real(548.8), real(213.0)),
			Vec3(real(343.0), real(548.8), real(343.0)),
			Vec3(real(213.0), real(548.8), real(343.0)),
			Vec3(real(213.0), real(548.8), real(213.0)));
		ls.scale(_scaleFactor);
		ls.translate(_t);
		ls.translate(Vec3(0.0f,-0.01f,0.0f));
		ls.updateAABB();
		return ls;
	}

	// Top of box
	static Quadrilateral createLightBlock1() {
		Quadrilateral lb1 = Quadrilateral(
			Vec3(real(0.0),		real(0.0),	real(0.0)),
			Vec3(real(4.0),		real(0.0),	real(0.0)),
			Vec3(real(4.0),		real(0.0),	real(4.0)),
			Vec3(real(0.0),		real(0.0),	real(4.0)));
		lb1.scale(_scaleFactor);
		lb1.translate(_t + _lightboxTranslate);
//		lb1.translate(
		lb1.updateAABB();
		return lb1;
	}
	static Quadrilateral createLightBlock2() {
		Quadrilateral lb2 = Quadrilateral(
			Vec3(real(0.0),		real(0.0),	real(0.0)),
			Vec3(real(4.0),		real(0.0),	real(0.0)),
			Vec3(real(0.0),		real(1.0),	real(0.0)),
			Vec3(real(4.0),		real(1.0),	real(0.0)));
		lb2.scale(_scaleFactor);
		lb2.translate(_t + _lightboxTranslate);
		lb2.updateAABB();
		return lb2;
	}
	static Quadrilateral createLightBlock3() {
		Quadrilateral lb3 = Quadrilateral(
			Vec3(real(130.0),	real(0.0),		real(65.0)),
			Vec3(real(130.0),	real(165.0),	real(65.0)),
			Vec3(real(290.0),	real(165.0),	real(114.0)),
			Vec3(real(290.0),	real(0.0),		real(114.0)));
		lb3.scale(_scaleFactor);
		lb3.translate(_t + _lightboxTranslate);
		lb3.updateAABB();
		return lb3;
	}
	static Quadrilateral createLightBlock4() {
		Quadrilateral lb4 = Quadrilateral(
			Vec3(real(82.0),	real(0.0),	real(225.0)),
			Vec3(real(82.0),	real(165.0),	real(225.0)),
			Vec3(real(130.0),	real(165.0),	real(65.0)),
			Vec3(real(130.0),	real(0.0),	real(65.0)));
		lb4.scale(_scaleFactor);
		lb4.translate(_t + _lightboxTranslate);
		lb4.updateAABB();
		return lb4;
	}
	static Quadrilateral createLightBlock5() {
		Quadrilateral lb5 = Quadrilateral(
			Vec3(real(240.0),	real(0.0),	real(272.0)),
			Vec3(real(240.0),	real(165.0),	real(272.0)),
			Vec3(real(82.0),	real(165.0),	real(225.0)),
			Vec3(real(82.0),	real(0.0),	real(225.0)));
		lb5.scale(_scaleFactor);
		lb5.translate(_t + _lightboxTranslate);
		lb5.updateAABB();
		return lb5;
	}



private:
		static float _scaleFactor;
		static Vec3 _t;
		static Vec3 _lightboxTranslate;
};


float CornellBoxFactory::_scaleFactor = real(0.025);
Vec3 CornellBoxFactory::_lightboxTranslate = Vec3(real(0.0),real(-5.0),real(20.0));
Vec3 CornellBoxFactory::_t = Vec3(real(-7),real(-6.8),real(15.6));//Vec3(-278.f,-273.f,800.f);
#endif
