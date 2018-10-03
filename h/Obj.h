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

#ifndef __PLAIN_OBJECT__
#define __PLAIN_OBJECT__

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

#include "GLProgram.h"

class Obj{
	public:
		using MapUint 	= std::unordered_map< std::string, GLuint >;
		using MapInt  	= std::unordered_map< std::string, GLint >;
		using MapVec3 	= std::unordered_map< std::string, glm::vec3 >;
		using MapMat4 	= std::unordered_map< std::string, glm::mat4 >;
		using MapProg 	= std::unordered_map< std::string, GLProgram >;
		
		// The Vertex Array Objects to hold our OpenGL state
		MapUint vao = {
				{ "main", GLuint{} }
			};

		// The Buffers to hold our data
		MapUint buffer = {
				{ "position", GLuint{} },
				{ "color", GLuint{} }
			};

		// The position of the object for animating
		MapVec3 position = {
				{ "now", glm::vec3{0.0f, 0.0f, 0.0f} },
				{ "prev", glm::vec3{0.0f, 0.0f, 0.0f} }
			};

		// The current velocity of the object and the speed to add to it
		MapVec3 movement = {
			{"vel", glm::vec3{0.0f, 0.0f, 0.0f} },
			{"speed", glm::vec3{0.0f, 0.0f, 0.0f} }
		};

		// Default radius of our object
		GLfloat radius = 5.0f;

		// The Uniform names in our shaders
		MapInt uniform = {
				{ "model", GLint{} },
				{ "view", GLint{} },
				{ "proj", GLint{} }
			};

		// The attribute names in our shaders
		MapInt attr = {
				{ "position", GLint{} },
				{ "color", GLint{} }
			};

		// The model, view, and projection to send to the shader program
		MapMat4 MVP = {
				{ "model", 	glm::translate(
		        				glm::mat4(1.0f),
		        				this->position["now"]
		    				)},
				{ "view",   glm::lookAt(
						    	glm::vec3(0.0f, 0.0f, 0.0f),
						    	glm::vec3(0.0f, 0.0f, 0.0f),
						    	glm::vec3(0.0f, 1.0f, 0.0f)
						    )},	
				{ "proj",   glm::perspective(
								glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f
							)},
				{ "ortho", 	glm::ortho(0.0f, 800.0f,  0.0f, 600.0f, 0.1f, 100.0f)}
			};

		// The OpenGL program
		MapProg program = {
				{ "simple", GLProgram{} }
			};

		// The number of vertexes in our position buffer
		GLint numVertices = 0;

		Obj(){};
		virtual ~Obj(){};

		/**********************************************
		*
		*				OpenGL
		*
		***********************************************/
		virtual Obj& getGLLocations() {
			return *this;
		};
		virtual Obj& fillBuffers() {
			return *this;
		};
		virtual Obj& setVAOState() {
			return *this;
		};
		virtual Obj& setMVP() {
			return *this;
		};
		virtual Obj& updateGL() {
			return *this;
		};

		virtual Obj& init() {
			return *this;
		};

		/**********************************************
		*
		*				Logic
		*
		***********************************************/
		virtual Obj& input(const SDL_Event& ev) {
			return *this;
		};
		virtual Obj& handleEdge() {
			return *this;
		};
		virtual Obj& handleMovement() {
			return *this;
		};
		virtual Obj& updatePosition(const float& dt=1) {
			return *this;
		};
		virtual Obj& collisions() {
			return *this;
		};
		virtual Obj& interpolate(const float& ip=1) {
			return *this;
		};

		/**********************************************
		*
		*				Draw
		*
		***********************************************/
		virtual Obj& draw() {
			return *this;
		};
};

#endif
