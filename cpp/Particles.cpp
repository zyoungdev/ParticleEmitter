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

#include <stdlib.h>
#include "../h/Particles.h"

Particles& Particles::init() {
    particles.reserve(numParticles);

	// Seed our random number generator
	// We aren't dealing with secure communications
	// So this will do fine
    std::srand(std::time(0));

    // Fill our vector with particles
	for (auto i = 0; i < this->numParticles; i++)
	{
		Particle particle;
		this->particles.push_back(particle);
	}

	// For each particle
	for (auto &p : this->particles)
	{
		// Set the radius of the particle randomly
		p.radius = (std::rand() % this->maxRadius + this->minRadius) / 100.0f;

		// Initialize the particle
		// See the Particle::init()
		p.init();

		// Set the initial position, and vertical and horizontal speed
		this->resetParticle(p);
	}

	return *this;
}
Particles& Particles::resetParticle(Particle& p)
{
	// Set both the previous position and now position to our emitter point
	p.position["now"].x = this->pos.x;
	p.position["prev"].x = this->pos.x;
	p.position["now"].y = this->pos.y;
	p.position["prev"].y = this->pos.y;

	// Set our velocity to zero so it has to start over
	p.movement["vel"].x = 0;
	p.movement["vel"].y = 0;

	// Set our vertical and horizontal speeds randomly
	p.movement["speed"].x = (std::rand() % this->maxSpeedX - (this->maxSpeedX / 2)) / 10.0f;
	p.movement["speed"].y = (std::rand() % this->maxSpeedY + this->minSpeedY);

	return *this;
}

/**********************************************
*
*				Logic
*
***********************************************/
Particles& Particles::input(const SDL_Event& ev) {
	return *this;
}
Particles& Particles::handleEdge() {
	// for each particle
	for (auto &p : this->particles)
	{
		// If the edge of the particle is below the screen bottom
		if (p.position["now"].y - p.radius < 0)
		{
			// Set the particle to rest on the screen bottom
			p.position["now"].y = p.radius;

			// Bounce the particle
			// Reverse the vertical direction and slow down our vertical speed
			p.movement["vel"].y *= -0.8f;
			// Slow down our horizontal speed
			p.movement["vel"].x *= 0.9f;

			// Reset Slow Particles
			if (abs(p.movement["vel"].x) < 50.0f && abs(p.movement["vel"].y < 50.0f))
				this->resetParticle(p);
		}

		// Reset particle when past left and right screen edges
		if (p.position["now"].x - p.radius > 800 || p.position["now"].x + p.radius < 0)
			this->resetParticle(p);
	}
	return *this;
}
Particles& Particles::handleMovement(const float& dt) {
	// For each particle
	for (auto &p : this->particles)
	{
		// Add the speed to our current velocity
		// Multiplying the speed by deltaTime will allow us to
		// Use speeds in pixels per second
		p.movement["vel"].x += p.movement["speed"].x * dt;
		p.movement["vel"].y += p.movement["speed"].y * dt;
	}

	return *this;
}
Particles& Particles::addGravity(const float& dt) {
	// for each particle
	for (auto &p : this->particles)
		// Remove speed due to gravity
		p.movement["speed"].y -= this->gravity * dt;

	return *this;
}
Particles& Particles::updatePosition(const float& dt) {

	this->handleMovement(dt)
		.addGravity(dt);

	// For each particle
	for (auto &p : this->particles)
	{
		// Set the current position based on the previous position and add the velocity
		// Multiplying the velocity by deltaTime allows us to set velocity in pixels per second
		p.position["now"].x = p.position["prev"].x + p.movement["vel"].x * dt;
		p.position["now"].y = p.position["prev"].y + p.movement["vel"].y * dt;
	}

	this->handleEdge();

	// For each particle
	for (auto &p : this->particles)
		// Set the previous position to the current position to test against on the next frame
		p.position["prev"] = p.position["now"];

	return *this;
}
Particles& Particles::collisions() {
	return *this;
}
Particles& Particles::interpolate(const float& dt, const float& ip) {
	// For each particle
	for (auto &p : this->particles)
	{
		// Do the same as in updatePosition() but utilize interpolation
		// This is merely showing our particles in the correct place in time
		p.position["now"].x = p.position["prev"].x + (p.movement["vel"].x * dt) * ip;
		p.position["now"].y = p.position["prev"].y + (p.movement["vel"].y * dt) * ip;
	}
	return *this;
}

/**********************************************
*
*				Draw
*
***********************************************/
Particles& Particles::draw() {
	// For each particle, draw
	for (auto &p : this->particles)
		p.draw();

	return *this;
}
