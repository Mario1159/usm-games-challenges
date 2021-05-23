#include "coin.hpp"

coin::coin(float x, float y, float w, float h, coin_type type, SDL_Renderer* renderer, SDL_Texture* texture) :
        x(x), y(y), w(w), h(h), type(type), renderer(renderer), texture(texture) {}

void coin::update(double delta_time) {
    speed_y -= 0.05f * delta_time;
    y -= speed_y;
    SDL_Rect rect{(int)x, (int)y, (int)w, (int)h};
    SDL_RenderCopy(renderer, texture, &(sprite_rects[type]), &rect);
}

