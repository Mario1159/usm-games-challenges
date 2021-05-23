#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

#include "coin.hpp"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
TTF_Font* font = nullptr;
std::pair<int, int>* mouse_click = nullptr;

SDL_Texture* coin_sheet = nullptr;
SDL_Texture* text_texture = nullptr;

std::vector<coin*> coins;
int cash = 0;

SDL_Texture* load_texture(std::string path) {
    SDL_Texture* texture = nullptr;
    SDL_Surface* surface = IMG_Load(path.c_str());
    if(surface == nullptr)
        std::cout << "Unable to load image: " << path.c_str() << std::endl << IMG_GetError() << std::endl;
    else {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if(texture == nullptr)
            std::cout << "Unable to create texture: " << path.c_str() << std::endl << SDL_GetError();
        SDL_FreeSurface(surface);
    }
    return texture;
}

SDL_Texture* load_text(TTF_Font* font, std::string text) {
    SDL_Texture* texture = nullptr;
    if (!font)
        std::cout << "Unable to load font:" << std::endl << TTF_GetError() << std::endl;
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), SDL_Color{0xFF, 0xFF, 0xFF, 0x00});
    if(surface == nullptr)
        std:: cout << "Unable to render text surface:" << std::endl << TTF_GetError() << std::endl;
    else {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if(texture == nullptr)
            std::cout << "Unable to create texture from rendered text" << std::endl << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
    }
    return texture;
}

void init() {
    unsigned int rseed = std::time(0);
    std::cout << "Seed: " << rseed << std::endl;
    std::srand(rseed);

    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

    coin_sheet = load_texture("../rsc/coins.png");
    font = TTF_OpenFont("../rsc/prstartk.ttf", 28);
}

double coin_clock = 0.0;

void update(double delta_time) {
    int win_w, win_h;
    SDL_GetWindowSize(window, &win_w, &win_h);

    coin_clock += delta_time;
    if(coin_clock > 0.1) {
        coin_clock = 0.0;

        coin::coin_type t;

        int p = std::rand() % 100;
        if(p < 85)
            t = coin::coin_type::one;
        else if(p < 95)
            t = coin::coin_type::five;
        else if(p < 100)
            t = coin::coin_type::ten;

        coins.push_back(new coin(std::rand() % (win_w - coin::sprite_width), -coin::sprite_height, 50, 50, t, renderer, coin_sheet));
    }

    std::vector<coin*>::iterator c = std::begin(coins);
    int mouse_x, mouse_y;
    while (c != std::end(coins)) {
        (*c)->update(delta_time);
        if ((*c)->y > win_h) {
            delete (*c);
            c = coins.erase(c);
        } else if (SDL_GetMouseState(&mouse_x, &mouse_y) & SDL_BUTTON(SDL_BUTTON_LEFT) &&
             std::sqrt(std::pow(mouse_x - ((*c)->x + (*c)->w / 2), 2) + std::pow(mouse_y - ((*c)->y + (*c)->h / 2), 2) < 120)) {
            switch((*c)->type){
                case coin::coin_type::one:
                    cash += 1;
                    break;
                case coin::coin_type::five:
                    cash += 5;
                    break;
                case coin::coin_type::ten:
                    cash += 10;
                    break;
            }
            delete (*c);
            c = coins.erase(c);
        } else
            ++c;
    }
    
    if(text_texture)
        SDL_DestroyTexture(text_texture);
    text_texture = load_text(font,"Cash: " + std::to_string(cash));
    SDL_Rect text_rect{(int)(win_w * 0.9), 0, (int)(win_w * 0.1), (int)(win_h * 0.05)};  
    SDL_RenderCopy(renderer, text_texture, nullptr, &text_rect);
}

int main(int argc, char* args[])
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
        std::cout << "SDL could not initialize:" << std::endl << SDL_GetError() << std::endl;
    else if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
        std::cout << "SDL_image could not initialize:" << std::endl << IMG_GetError() << std::endl;
    else if(TTF_Init() == -1)
        std::cout << "SDL_ttf could not initialize:" << std::endl << TTF_GetError() << std::endl;
    else {
        window = SDL_CreateWindow("Coin Rain!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE);
        if(!window)
            std::cout << "Window could not be created:" << std::endl << SDL_GetError() << std::endl;
        else {
            bool is_running = true;
            Uint64 start_time = SDL_GetPerformanceCounter();
            Uint64 end_time = start_time;
            double delta_time = 0;

            SDL_Event events;

            init();

            while(is_running) {

                SDL_RenderClear(renderer);

                while(SDL_PollEvent(&events) != 0)
                    if(events.type == SDL_QUIT)
                        is_running = false;

                update(delta_time);

                SDL_RenderPresent(renderer);

                start_time = end_time;
                end_time = SDL_GetPerformanceCounter();
                delta_time = (end_time - start_time) / (double)SDL_GetPerformanceFrequency();
            }
        }
    }
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
