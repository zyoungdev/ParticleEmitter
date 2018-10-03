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

#include "../h/SDLWindow.h"
#include "../h/GameLoop.h"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../h/Particles.h"


class myGameLoop :
    public GameLoop
{
    using GameLoop::GameLoop;

private:
	Particles particles;


public:
    virtual void init()
    {
    	particles.init();
    }

    virtual void console_output()
    {
        GameLoop::console_output();
    }

    virtual void inputs(SDL_Event& events)
    {
        if (events.type == SDL_KEYDOWN &&
                events.key.keysym.sym == SDLK_SPACE)
            toggle_pause();
    }

    virtual void update_positions(const float& delta)
    {
        particles.updatePosition(delta);
    }

    virtual void collisions()
    {}

    virtual void interpolate(const float& delta, const float& interpolation)
    {
        particles.interpolate(delta, interpolation);
    }

    virtual void draw(){
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        particles.draw();

        SDL_GL_SwapWindow(win);
    }
};

int main (int argc, char* argv[])
{
    SDLWindow win("My Game", 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    glewExperimental = GL_TRUE;
    glewInit();

    myGameLoop myGame(30, myGameLoop::INTERPOLATIONS::FOUR);

    myGame.start(win);

    return 0;
}   
