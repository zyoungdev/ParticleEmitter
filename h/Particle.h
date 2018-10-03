/*
  This file is part of Particles.

  fct is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  fct is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with fct.  If not, see <http://www.gnu.org/licenses/>.

  Copyright 2018 Zachary Young
  */

#ifndef __SINGLE_PARTICLE__
#define __SINGLE_PARTICLE__

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

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
