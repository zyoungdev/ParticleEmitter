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

#ifndef _SDL_WINDOW_
#define _SDL_WINDOW_

#include <SDL2/SDL.h>
#include <iostream>
#include <string>

class SDLWindow{
public:
    SDL_Window* win = nullptr;
    SDL_Renderer* rend = nullptr;

private:
    SDL_GLContext glContext = nullptr;

public:
    SDLWindow(
        const std::string& n = "Game Loop",
        const uint& w = 800,
        const uint& h = 600,
        const Uint32& o = SDL_WINDOW_SHOWN,
        const Uint32& r_o = SDL_RENDERER_ACCELERATED)
    {
        if (initSDL())
        {
            initWindow(n, w, h, o);

            if ((o & SDL_WINDOW_OPENGL) == 0)
                initRenderer(r_o);
        }
    }

    ~SDLWindow()
    {
        if (glContext != nullptr)
            SDL_GL_DeleteContext(glContext);

        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
    }

private:
    bool initSDL()
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
          std::cout << "could not initialize SDL2 \n" << SDL_GetError() << "\n";
          return false;
        }

        return true;
    }

    SDLWindow& initWindow(const std::string& name,
        const Uint32& width,
        const Uint32& height,
        const Uint32& opts)
    {
        win = SDL_CreateWindow((const char*)name.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            opts);

        if (win == nullptr)
            std::cout << "Could not create Window\n" << SDL_GetError() << "\n";

        if ((opts & SDL_WINDOW_OPENGL) == SDL_WINDOW_OPENGL)
        {
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

            glContext = SDL_GL_CreateContext(win);

            SDL_GL_SetSwapInterval(0);
        }

        return *this;
    }

    SDLWindow& initRenderer(const Uint32& opts)
    {
        rend = SDL_CreateRenderer(win, -1,
                opts);

        if (rend == nullptr)
            std::cout << "Could not create Renderer\n" << SDL_GetError() << "\n";

        return *this;
    }
};

#endif
