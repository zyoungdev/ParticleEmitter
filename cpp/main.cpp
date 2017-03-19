#include "../h/SDLWindow.h"
#include "../h/GameLoop.h"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../h/Particles.h"


class myGameLoop: public GameLoop{
private:
	Particles particles;

	GLboolean isStarted = false;
public:
    using GameLoop::GameLoop;
    virtual myGameLoop& init(){
    	particles.init();
        return *this;
    }
    virtual myGameLoop& consoleOutput(){
        GameLoop::consoleOutput();

        return *this;
    }
    virtual myGameLoop& inputs(){
        switch (this->e.type) {
            case SDL_KEYDOWN: {

            	// On key press, start the animation
            	this->isStarted = true;

                break;
            }
        }
        return *this;
    }
    virtual myGameLoop& updatePositions(){

        if (this->isStarted)
	    	particles.updatePosition(this->deltaTime);

        return *this;
    }
    virtual myGameLoop& collisions(){

        return *this;
    }
    virtual myGameLoop& interpolate(){
    	
    	if (this->isStarted)
    		particles.interpolate(this->deltaTime, this->interpolation);

        return *this;
    }
    virtual myGameLoop& draw(){
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        particles.draw();

        SDL_GL_SwapWindow(this->win);
        return *this;
    }
};

int main (int argc, char* argv[])
{
    SDLWindow win("My Game", 800, 600, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);


    glewExperimental = GL_TRUE;
    glewInit();


    myGameLoop myGame(30, "slow");

    myGame.start(win);
    return 0;
}   