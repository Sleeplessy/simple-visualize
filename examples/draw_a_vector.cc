
/*
  libSDL2pp - C++11 bindings/wrapper for SDL2
  Copyright (C) 2013-2015 Dmitry Marakasov <amdmi3@amdmi3.ru>
  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include "../visualize.hpp"
#include <iostream>
#include <vector>
#include <SDL2pp/Window.hh>


using namespace SDL2pp;

int main(int, char *[]) try {
    SDL sdl(SDL_INIT_VIDEO);
    Window window("libSDL2pp demo: sprites", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                  SDL_WINDOW_RESIZABLE);
    Renderer render(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    render.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
    std::vector<int> v{1, 3, 3, 4, 5, 11, 7, 8, 17}; // init a data array
    while (1) {
        // Process input
        SDL_Event event;
        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_q)))
                return 0;

        // Clear screen
        render.SetDrawColor(0, 32, 32);
        render.Clear();
        visualize::drawList(v,render); // draw it!
        render.Present();
        // Frame limiter
        SDL_Delay(1);
    }

    return 0;
} catch (std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
}
