#pragma once

#include <SDL.h>

#include <array>

class coin {
    public:
        enum coin_type {
            one,
            five,
            ten
        };
        coin(float x, float y, float w, float h, coin_type type, SDL_Renderer* renderer, SDL_Texture* texture);

        float x, y, w, h;
        coin_type type = coin_type::one;
        SDL_Renderer* renderer;
        SDL_Texture* texture;

        void update(double delta_time);

        static const int sprite_width = 108;
        static const int sprite_height = 108;

    private:
        float speed_y = 0.f;

        static constexpr std::array<SDL_Rect, 3> sprite_rects = [] {
            std::array<SDL_Rect, 3> r{};
            for(int i = 0; i < 3; i++)
                r[i] = SDL_Rect{sprite_width * i, 0, sprite_width, sprite_height};
            return r;
        }();
};
