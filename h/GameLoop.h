#ifndef _GAME_LOOP_
#define _GAME_LOOP_

#include <SDL2/SDL.h>
#include <iostream>
#include <string>

class GameLoop{
private:
    int updatesPerSecond = 60;
    int singleFrameTimeInMS = 0;
    const int maxFrameSkip = 5;
    int loops = 0; 

    Uint32 nextFrameTime = 0;
    Uint32 prevFrameTime = 0;

    std::string intpolSpeed = "medium";
    int prevIntpol = -1;

    Uint32 timerNow = 0;
    Uint32 drawCount = 0;

    bool isFirstRun = true;

    
    GameLoop& setUPS(const uint& u){
        if (u > 0 && u <= 240)
        {
            this->updatesPerSecond = u;
            
            if (u % 2 == 0)
                this->updatesPerSecond--;

            this->singleFrameTimeInMS = int(1000 / this->updatesPerSecond);
        }
        return *this;
    }
    GameLoop& checkForQuit(){
        switch (this->e.type)
        {
            case SDL_QUIT:
            {
                isRunning = false;
                break;
            }
            case SDL_WINDOWEVENT:
            {
                if (this->e.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
                {
                    SDL_Event f;
                    f.type = SDL_WINDOWEVENT;
                    f.window.event = SDL_WINDOWEVENT_FOCUS_GAINED;
                    SDL_WaitEvent(&f);
                }
                break;
            }
        }
        return *this;
    }
    GameLoop& intAndDraw(const int& i)
    {
        this->interpolate()
            .draw();
        this->prevIntpol = i;
        this->drawCount++;
        return *this;
    }
    GameLoop& mainLoop()
    {
        //Start the loop while isRunning is true
        while (this->isRunning)
        {
            this->now = SDL_GetTicks() / 1000.0f;
            this->consoleOutput();
            //This function holds our input handling
            //We can also put this above the updatePositions() call
            //But that slows down the inputs.
            while (SDL_PollEvent(&this->e))
            {
                this->checkForQuit()
                    .inputs()
                    .collisions();
            }

            //loops is the number of time we have skipped frames
            this->loops = 0;
            // a way to allow us to specify the velocity of our 
            //objects in pixels per second. This keeps the animation distance
            //constant when the frame rate changes
            this->deltaTime = float(SDL_GetTicks() - this->prevFrameTime) / 1000.0f;

            //This loop determines when to update the position
            //This is how we separate the drawing and the position updating
            while( SDL_GetTicks() > this->nextFrameTime && this->loops < this->maxFrameSkip) 
            {
                //Finally update the position of our objects
                this->updatePositions();

                //nextFrameTime is the time in MS we need to pass to update
                this->nextFrameTime += this->singleFrameTimeInMS;
                //This is the time right now
                this->prevFrameTime = SDL_GetTicks();
                //If we are stuck here because the frame rate slows we need to
                //break out if loops is > maxFrameSkip
                this->loops++;
            }
            //This is basically the percentage between frames we currently are
            this->interpolation = float( SDL_GetTicks() + this->singleFrameTimeInMS - this->nextFrameTime )
                / float( this->singleFrameTimeInMS );

            //convert that percentage to an integer to make it easy to test
            int ip = int(this->interpolation * 100);

            if (ip < 10)
                ip = 0;
            //Finally we can draw if the following tests have passed
            //Check against previous interpolation so we don't render
            //the same thing more than once
            //Play around with these to find an ideal interpolation
            if ( (this->intpolSpeed == "slow" || this->intpolSpeed == "off") &&
                (ip != this->prevIntpol && (ip == 0 || ip == 50)) )
            {
                if (ip == 50 && this->intpolSpeed == "off")
                    this->prevIntpol = ip;
                else
                    this->intAndDraw(ip);
            }
            else if ( (this->intpolSpeed == "medium") &&
                (ip != this->prevIntpol && (ip == 0 || ip == 25 || ip == 50 || ip == 75)) )
            {
                this->intAndDraw(ip);
            }
            else if (intpolSpeed == "fast")
            {
                this->intAndDraw(ip);
            }
        }
        return *this;
    }

public:
    GameLoop(const uint& ups=60, const std::string& speed="medium"){
        this->setUPS(ups);

        if (speed == "off" || speed == "slow" || speed == "medium" || speed == "fast")
            this->intpolSpeed = speed;

    }   
    virtual ~GameLoop(){}
    
    SDL_Event e;
    SDL_Window* win = NULL;

    float now = 0.0;
    float deltaTime = 0.0;
    float interpolation = 0.0;
    bool isRunning = true;

    GameLoop& start(){
        this->init()
            .mainLoop();

        return *this;
    }
    GameLoop& start(SDLWindow& sdlWin){
        if (sdlWin.win != NULL)
            this->win = sdlWin.win;

        this->start();

        return *this;
    }
    
    virtual GameLoop& init(){
        return *this;
    }
    virtual GameLoop& consoleOutput(){
        //To show what is happening in the terminal
        //this is a timer that shows the number of times our interpolated 
        //objects have been drawn every second

        if (SDL_GetTicks() > this->timerNow + 1000)
        {
            if (this->isFirstRun)
                std::cout << "Frames Drawn\tDelta\t\tInterpolation\n";
            this->isFirstRun = false;

            std::cout << this->drawCount << "\t\t" << this->deltaTime << "\t\t" << this->interpolation << "\n";
            this->drawCount = 0;
            this->timerNow = SDL_GetTicks();
        }
        return *this;
    }
    virtual GameLoop& inputs(){
        return *this;
    }
    virtual GameLoop& collisions(){
        return *this;
    }
    virtual GameLoop& updatePositions(){
        return *this;
    }
    virtual GameLoop& interpolate(){
        return *this;
    }
    virtual GameLoop& draw(){
        return *this;
    }
};


#endif