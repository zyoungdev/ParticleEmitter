#ifndef _SDL_WINDOW_
#define _SDL_WINDOW_

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <thread>

class SDLWindow{
public:
    SDL_Renderer* rend = NULL;
private:
    SDL_GLContext glContext = NULL;
    SDL_Event e;
    bool isRunning = true;
    std::thread quitEventThread;

    SDLWindow& initSDL(){
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
          std::cout << "could not initialize SDL2 \n" << SDL_GetError() << "\n";
        }
        return *this;
    }
    SDLWindow& initWindow(const std::string& name, 
        const uint& width, 
        const uint& height, 
        const int& opts)
    {
        this->win = SDL_CreateWindow((const char*)name.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            opts);

        if (this->win == NULL)
            std::cout << "Could not create Window\n" << SDL_GetError() << "\n";

        if ((opts & SDL_WINDOW_OPENGL) == SDL_WINDOW_OPENGL)
        {
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

            glContext = SDL_GL_CreateContext(win);
        }

        return *this;
    }
    SDLWindow& initRenderer(){
        this->rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED||SDL_RENDERER_PRESENTVSYNC);

        if (this->rend == NULL)
        {
            std::cout << "Could not create Renderer\n" << SDL_GetError() << "\n";
        }
        return *this;
    }

public:
    SDLWindow(const std::string& n="Game Loop", const uint& w=800, const uint& h=600, const int& o=SDL_WINDOW_SHOWN){
        this->initSDL()
            .initWindow(n, w, h, o)
            .initRenderer();
    }
    ~SDLWindow(){
        if (glContext != NULL)
            SDL_GL_DeleteContext(glContext);
        
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
    }

    SDL_Window* win = NULL;
};

#endif