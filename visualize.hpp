#ifndef __DRAW_ELEM__
#define __DRAW_ELEM__

#include <SDL.h>
#include <SDL2pp/SDL.hh>
#include <SDL2pp/Renderer.hh>
#include <iterator>
#include <algorithm>

namespace visualize {
    template<typename T>
    void drawListElement(T elem, unsigned index, unsigned total, unsigned highest , SDL2pp::Renderer &render, const SDL2pp::Color &color = {6, 202, 72}) {
        static_assert(std::is_arithmetic<T>::value,"Only arithmetic types are allowed to define graphs");
        int s_width = render.GetOutputWidth();
        int s_height = render.GetOutputHeight() - 5 ;
        int elem_width = s_width / total;
        int elem_height = ((s_height / highest) * elem) ;
        int x = index * elem_width;
        render.SetDrawColor(color);
        render.FillRect(x, s_height, x + elem_width, s_height - elem_height); // FILL COLOR
        render.SetDrawColor(255, 255, 255);
        render.DrawRect(x, s_height, x + elem_width, s_height - elem_height); // DRAW BORDER
    }


    template<std::size_t scale_x = 6, std::size_t scale_y =6,typename T,template <typename, typename> class Container>
    void drawList(Container<T, std::allocator<T>> elem_list, SDL2pp::Renderer &render, std::size_t current = 0,
                  const SDL2pp::Color cur_color = {255, 153, 51},
                  const SDL2pp::Color default_color = {6, 202, 72}) // Colors
    {
        std::for_each(std::begin(elem_list), std::end(elem_list),
                      [scale_x = scale_x, scale_y = scale_y, iter = std::begin(
                              elem_list), &elem_list, &render, current, cur_color, default_color]
                              (T n) mutable {
                          unsigned cur_distance = std::distance(std::begin(elem_list), iter++);
                          !std::is_sorted(std::begin(elem_list), std::end(elem_list)) ?
                          drawListElement(n * scale_y * 0.125, cur_distance,
                                          std::distance(std::begin(elem_list),std::end(elem_list)) / (scale_x * 0.125), // scale for width
                                          *std::max_element(std::begin(elem_list), std::end(elem_list)), render,
                                          (current == cur_distance) ? cur_color : default_color
                          ) : drawListElement(n * scale_y * 0.125, cur_distance,
                                              std::distance(std::begin(elem_list), std::end(elem_list)) /
                                              (scale_x * 0.125), // scale for width
                                              *std::max_element(std::begin(elem_list), std::end(elem_list)), render,
                                              default_color
                          );
                      }
        );
    }
}
#endif
