#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <GL/glfw.h>
#include <iostream>
#include <vector>
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
