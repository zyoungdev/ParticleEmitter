#ifndef __SINGLE_PARTICLE__
#define __SINGLE_PARTICLE__

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "GLProgram.h"
#include "Obj.h"

// Inherit the public and protected member of Obj
class Particle : public Obj {
public:
	virtual ~Particle();

	virtual Particle& init();
	/**********************************************
	*
	*				OpenGL
	*
	***********************************************/
	virtual Particle& getGLLocations();
	virtual Particle& compileShaders();
	virtual Particle& fillBuffers();
	virtual Particle& setVAOState();
	virtual Particle& setMVP();
	virtual Particle& updateGL();
	virtual Particle& deleteBuffers();
	virtual Particle& deleteVertexArrays();
	/**********************************************
	*
	*				Logic
	*
	***********************************************/
	virtual Particle& input(const SDL_Event& ev);
	virtual Particle& handleEdge();
	virtual Particle& handleMovement();
	virtual Particle& updatePosition(const float& dt=1);
	virtual Particle& collisions();
	virtual Particle& interpolate(const float& ip=1);

	/**********************************************
	*
	*				Draw
	*
	***********************************************/
	virtual Particle& draw();
};


#endif