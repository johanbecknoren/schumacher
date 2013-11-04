#ifndef GLRENDERER_H
#define GLRENDERER_H
#ifdef USE_OPENGL

#include <iostream>
#include <vector>
#include <GL/glfw.h>

class GlRenderer {
public:
	GlRenderer(const int W, const int H);
	~GlRenderer();	
	void render(int *image);
	void render(float *image, std::vector<bool> *done);
	GLubyte *texture;
	GLuint texId;
	const int _W;
	const int _H;
};
#endif
#endif
