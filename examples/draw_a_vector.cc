
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
#include <vector>
#include <SDL2pp/Window.hh>
#include <thread>
#include <chrono>
#include <mutex>
#include <random>
#include <future>

using namespace SDL2pp;

std::vector<int> random_list(unsigned size = 100) {
    std::random_device rnd_device;
    // Specify the engine and distribution.
    std::mt19937 mersenne_engine(rnd_device());
    std::uniform_int_distribution<int> dist(1, 52);

    auto gen = std::bind(dist, mersenne_engine);
    std::vector<int> vec(size);
    generate(vec.begin(), vec.end(), gen);
    return vec;
}

int main(int, char *[]) try {

    SDL sdl(SDL_INIT_VIDEO);
    Window window("simple-visualization insert sort", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 900, 600,
                  SDL_WINDOW_RESIZABLE);
    Renderer render(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    render.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
    render.SetDrawColor(0, 0, 0);
    render.Clear();
    render.Present();

    std::condition_variable cv;
    std::condition_variable cv_update;
    std::condition_variable cv_start;
    std::mutex m;
    std::mutex m2;
    std::unique_lock<std::mutex> lk(m);
    std::unique_lock<std::mutex> start(m2);
    std::vector<int> v = random_list(120); // init a data array
    std::size_t current = 0;
    std::thread t([&]() {
                      while (1) {
                          cv.wait(lk);
                          visualize::drawList<8, 8>(v, render, current); // draw it!
                          render.Present();
                          cv_update.notify_one();
                          std::this_thread::yield();
                      }
                  }
    );
    std::thread t1([&v, &cv, &current, &lk, &cv_start, &start]() { // sorting thread
        for (auto i = v.begin(); i != v.end(); ++i) {
            std::rotate(std::upper_bound(v.begin(), i, *i), i, i + 1); // insertion sort
            current = std::distance(v.begin(), i);
            cv.notify_one();
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }
        cv.notify_all();
    });
    t.detach();
    t1.detach();
    while (1) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_q)))
                return 0;
        }
        cv_update.wait(lk);
        render.SetDrawColor(0, 0, 0);
        render.Clear();
        SDL_Delay(1);
    }


    return 0;
} catch (std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
}
