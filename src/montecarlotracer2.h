#ifndef MONTECARLO2_H
#define MONTECARLO2_H
#include "tracer.h"
#include "octree.h"
#include <mutex>
#include "SFMT.h"
#include "glrenderer.h"
#include "quadrilateral.h"

class MonteCarloRayTracer2 : Tracer {
public:
	MonteCarloRayTracer2(const int W, const int H, const Quadrilateral lightQuad) : Tracer(W, H),
													working(true),
													_rayCounter(0),
													_meanRayDepth(0),
													_maxDepth(7),
													_minDepth(6),
													_raysPerPixel(12),
													_lightQuad(lightQuad)
	{
		_buffer = new float[W*H*3];
	};
	void render(float *pixels, Octree *tree, Camera *cam, bool singleThread, bool renderDuring);	

private:
	Vec3 iterateRay(Ray &ray, const Octree &tree, int depth, bool kill);
	Vec3 iterateGlassToAir(Ray &ray, const Octree &tree, int depth, bool kill);
	Vec3 iterateAirToGlass(Ray &ray, const Octree &tree, int depth, bool kill);
	
	struct Rng
	{
		unsigned int m_z, m_w;   
		Rng(unsigned int z = 362436069, unsigned int w = 521288629) : m_z(z), m_w(w) { }
		// Returns a 'canonical' float from [0,1)
		float nextFloat()
		{
			unsigned int i = nextUInt32();
			return i * 2.328306e-10f;
		}
		// Returns an int with random bits set
		unsigned int nextUInt32()
		{
			m_z = 36969 * (m_z & 65535) + (m_z >> 16);
			m_w = 18000 * (m_w & 65535) + (m_w >> 16);
			return (m_z << 16) + m_w; /* 32-bit result */
		}
	};
    struct ThreadData {
        int tId;
        int row;
        const int NUM_THREADS;
        ThreadData(int i, int row, const int NUM_THREADS) : tId(i), row(row), NUM_THREADS(NUM_THREADS) {};
    };
	void glRender(float *pixels);

	void threadRender(float *pixels, const Octree &tree, 
        const Camera &cam, ThreadData thd);
	bool working;
	void addToCount();
	void addToMeanDepth(int d);
	void testTimers();

    std::mutex _mutex;
	std::mutex _depthMutex;
	std::mutex _renderMutex;
	volatile int _rayCounter;
	sfmt_t _randomGenerator;
	Rng _rgen;
	int _meanRayDepth;
	int _maxDepth;
	int _minDepth;
	int _raysPerPixel;
	Quadrilateral _lightQuad;
	float *_buffer;
	};


#endif
