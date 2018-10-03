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

#include "../h/Particle.h"

Particle::~Particle() {
	// When the particle leaves the stack we make room
	// in the OpenGL server
	this->deleteBuffers()
		.deleteVertexArrays();
}

Particle& Particle::init() {
	this->compileShaders()
		.fillBuffers()
		.getGLLocations()
		.setVAOState()
		.setMVP();

	return *this;
}


/**********************************************
*
*				OpenGL
*
***********************************************/
Particle& Particle::compileShaders() {
	GLProgram temp{
	    {
	        GLShader{GL_VERTEX_SHADER, "glsl/vertex.glsl"},
	        GLShader{GL_FRAGMENT_SHADER, "glsl/fragment.glsl"}
	    }
	};
	this->program["simple"] = temp;
	return *this;
}
Particle& Particle::fillBuffers() {
	// Temporary vectors of data
	std::unordered_map< std::string, std::vector< GLfloat > > data = {
		{ "position", std::vector< GLfloat >{} },
		{ "color", std::vector< GLfloat >{} }
	};

	// Numbers of triangles in our circle
	GLint segments = 10;

	// The size of the outer edge of our triangle
	GLfloat slice = M_PI * 2 / segments;


	// Create our circle vertices
	// Fill our color data with white
	for (auto i = 0; i < segments + 1; i++)
	{
	    GLfloat angle = (GLfloat)i * slice;
	    GLfloat x = cos(angle) * this->radius + this->position["now"].x;
	    GLfloat y = sin(angle) * this->radius + this->position["now"].y;
	    GLfloat z = 0;

	    data["position"].push_back(x);
	    data["position"].push_back(y);
	    data["position"].push_back(z);

	    data["color"].push_back(1.0f);
	    data["color"].push_back(1.0f);
	    data["color"].push_back(1.0f);
	}

	this->numVertices = data["position"].size();


	// Create our vertex array Object
	glGenVertexArrays(1, &this->vao["main"]);

	// Create and fill our position buffer
	glGenBuffers(1, &this->buffer["position"]);
	glBindBuffer(GL_ARRAY_BUFFER, this->buffer["position"]);
	glBufferData(GL_ARRAY_BUFFER, data["position"].size() * sizeof(GLfloat), data["position"].data(), GL_STATIC_DRAW);

	//Create and fill our color buffer
	glGenBuffers(1, &this->buffer["color"]);
	glBindBuffer(GL_ARRAY_BUFFER, this->buffer["color"]);
	glBufferData(GL_ARRAY_BUFFER, data["color"].size() * sizeof(GLfloat), data["color"].data(), GL_STATIC_DRAW);

	return *this;
}
Particle& Particle::getGLLocations() {
	// Make sure we're using our shader program
	this->program["simple"].program_start();

	// Get the names of the attributes in our shader program
	this->attr["position"] = glGetAttribLocation(this->program["simple"].program(), "position");
	this->attr["color"] = glGetAttribLocation(this->program["simple"].program(), "color");

	// Get the names of the uniforms in our shader program
	this->uniform["model"] = glGetUniformLocation(this->program["simple"].program(), "model");
	this->uniform["view"] = glGetUniformLocation(this->program["simple"].program(), "view");
	this->uniform["proj"] = glGetUniformLocation(this->program["simple"].program(), "proj");

	// Stop using our shader program
	this->program["simple"].program_stop();
	return *this;
}
Particle& Particle::setVAOState() {
	// Set up the OpenGL state every time we bind our Vertex Attribute array
	// Do this once and OpenGL will do the rest
	glBindVertexArray(this->vao["main"]);
	    glBindBuffer(GL_ARRAY_BUFFER, this->buffer["position"]);
	    glEnableVertexAttribArray(this->attr["position"]);
	    glVertexAttribPointer(this->attr["position"], 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	    glBindBuffer(GL_ARRAY_BUFFER, this->buffer["color"]);
	    glEnableVertexAttribArray(this->attr["color"]);
	    glVertexAttribPointer(this->attr["color"], 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	return *this;
}
Particle& Particle::setMVP() {
	// Setup our Model View and Projection
	this->MVP = {
			{ "model", 	glm::translate(
	        				glm::mat4(1.0f),
	        				this->position["now"]
	    				)},
			{ "view",   glm::lookAt(
					    	glm::vec3(0.0f, 0.0f, 1.0f),
					    	glm::vec3(0.0f, 0.0f, 0.0f),
					    	glm::vec3(0.0f, 1.0f, 0.0f)
					    )},
			{ "proj",   glm::perspective(
							glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f
						)},
			// We will be using an orthographic projection since we are doing a 2D animation
			{ "ortho", 	glm::ortho(0.0f, 800.0f,  0.0f, 600.0f, 0.1f, 100.0f)}
		};
	return *this;
}
Particle& Particle::updateGL() {
	// Send the Model, View, and Projection to the OpenGL server
	// We do this every time we start using the program which is on every draw
	this->MVP["model"] = glm::translate(
        glm::mat4(1.0f),
        this->position["now"]
    );

	glUniformMatrix4fv(this->uniform["model"], 1, GL_FALSE, glm::value_ptr(this->MVP["model"]));
	glUniformMatrix4fv(this->uniform["view"], 1, GL_FALSE, glm::value_ptr(this->MVP["view"]));
	// Note the orthographic projection
	glUniformMatrix4fv(this->uniform["proj"], 1, GL_FALSE, glm::value_ptr(this->MVP["ortho"]));

	return *this;
}
Particle& Particle::deleteBuffers() {
	for (auto &el : this->buffer)
		glDeleteBuffers(1, &el.second);

	return *this;
}
Particle& Particle::deleteVertexArrays() {
	for (auto &el : this->vao)
		glDeleteVertexArrays(1, &el.second);

	return *this;
}
/**********************************************
*
*				Logic
*
***********************************************/
Particle& Particle::input(const SDL_Event& ev) {
	return *this;
}
Particle& Particle::handleEdge() {
	return *this;
}
Particle& Particle::handleMovement() {
	return *this;
}
Particle& Particle::updatePosition(const float& dt) {
	return *this;
}
Particle& Particle::collisions() {
	return *this;
}
Particle& Particle::interpolate(const float& ip) {
	return *this;
}

/**********************************************
*
*				Draw
*
***********************************************/
Particle& Particle::draw() {

	// Start using our program
	this->program["simple"].program_start();

	// Set the OpenGL server state
	glBindVertexArray(this->vao["main"]);

	// Send our MVP to the OpenGL server
	this->updateGL();

	// Draw our triangles in a fan to create our circle
	glDrawArrays(GL_TRIANGLE_FAN, 0, this->numVertices * 3);


	// Unbind the VAO and stop using the program so other objects
	// can use the server
	glBindVertexArray(0);
	this->program["simple"].program_stop();

	return *this;
}
