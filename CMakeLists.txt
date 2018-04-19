cmake_minimum_required(VERSION 3.9)
project(algo)

set(CMAKE_CXX_STANDARD 14)

SET(SDL2PP_WITH_IMAGE ON) # if you need SDL_image support
SET(SDL2PP_WITH_MIXER ON) # if you need SDL_mixer support
SET(SDL2PP_WITH_TTF ON) # if you need SDL_ttf support
ADD_SUBDIRECTORY(extlib/libSDL2pp)
INCLUDE_DIRECTORIES(${SDL2PP_INCLUDE_DIRS})
add_executable(ex_draw_vector examples/draw_a_vector.cc)

TARGET_LINK_LIBRARIES(ex_draw_vector ${SDL2PP_LIBRARIES})