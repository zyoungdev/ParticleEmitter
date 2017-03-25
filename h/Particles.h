#ifndef __PARTICLE_EMITTER__
#define __PARTICLE_EMITTER__

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

#include "Particle.h"

class Particles{
public:
	// A vector and holds all of our particles
	std::vector< Particle > particles;

	// The position of our Emitter
	glm::vec3 pos = {400.0f, 50.0f, 0.0f};

	// The number of particles to emit
	GLint numParticles = 750;

	// The maximum radius of each particle
	// This number is divided by 100.0f to get more variety in size
	GLint maxRadius = 1000;
	GLint minRadius = 250;

	// The rate we will subtract from our particles vertical velocity each frame
	GLfloat gravity = 750.0f;

	// The maximum speed our particles move horizontally
	// This value will be cut in half to make our particles move left and right of the origin
	GLint maxSpeedX = 800;

	// The minimum and maximum initial vertical speed
	GLint minSpeedY = 300;
	GLint maxSpeedY = 450;

	Particles& init();
	Particles& resetParticle(Particle& p);

	/**********************************************
	*
	*				Logic
	*
	***********************************************/
	Particles& input(const SDL_Event& ev);
	Particles& handleEdge();
	Particles& handleMovement(const float& dt = 1);
	Particles& addGravity(const float& dt = 1);
	Particles& updatePosition(const float& dt = 1);
	Particles& collisions();
	Particles& interpolate(const float& dt = 1, const float& ip = 1);

	/**********************************************
	*
	*				Draw
	*
	***********************************************/
	Particles& draw();
};

#endif
