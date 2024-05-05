#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include <coreinit/debug.h>
#include <coreinit/memory.h>
#include <nn/act.h>
#include <vpad/input.h>

#include <fa-solid-900_ttf.h>
#include <ter-u32b_bdf.h>
#include "main.hpp"



void draw_background(SDL_Renderer* renderer, int r, int g, int b, int a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);
}

void draw_rectangle(SDL_Renderer* renderer, int x, int y, int w, int h, int r, int g, int b, int a) {
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderFillRect(renderer, &rect);
}

void draw_text(SDL_Renderer* renderer, const char* text, int x, int y, int size, SDL_Color color = {255, 255, 255, 255}) {
    void* font_data = nullptr;
    uint32_t font_size = 0;
    OSGetSharedData(OS_SHAREDDATATYPE_FONT_STANDARD, 0, &font_data, &font_size);

    TTF_Font* font = TTF_OpenFontRW(SDL_RWFromMem((void*)font_data, font_size), 1, size);
    if (font == NULL) {
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    if (surface == NULL) {
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        return;
    }

    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

int get_text_width(const char* text, int size) {
    void* font_data = nullptr;
    uint32_t font_size = 0;
    OSGetSharedData(OS_SHAREDDATATYPE_FONT_STANDARD, 0, &font_data, &font_size);

    TTF_Font* font = TTF_OpenFontRW(SDL_RWFromMem((void*)font_data, font_size), 1, size);
    if (font == NULL) {
        return 0;
    }

    int width = 0;
    int height = 0;
    TTF_SizeText(font, text, &width, &height);

    TTF_CloseFont(font);
    return width;
}

void draw_menu_screen(SDL_Renderer* renderer) {
    draw_background(renderer, 16, 16, 16, 255);

    // Top Bar
    draw_rectangle(renderer, 0, 0, 1920, 100, 153, 0, 153, 255);
    draw_text(renderer, "Wii U Account Swap", 64, 10, 60);
    draw_text(renderer, APP_VERSION, 64 + get_text_width("Wii U Account Swap", 60) + 16, 10, 60, {176, 176, 176, 255});
    draw_text(renderer, "Nightkingale", 1920 - 64 - get_text_width("Nightkingale", 60), 10, 60);

    // Bottom Bar
    draw_rectangle(renderer, 0, 880, 1920, 200, 153, 0, 153, 255);
    draw_text(renderer, "Current User: ", 64, 880 + 10, 60);
    draw_text(renderer, MII_NICKNAME.c_str(), 64 + get_text_width("Current User: ", 60), 880 + 10, 60, {176, 176, 176, 255});
    draw_text(renderer, ACCOUNT_FILE.c_str(), 64, 980, 60);

    // Main Menu
    draw_text(renderer, "Press (A) to switch to Nintendo Network ID.", 64, 200, 30);
    draw_text(renderer, "Press (B) to switch to Pretendo Network ID.", 64, 300, 30);
    draw_text(renderer, "Press (+) to backup your current account.", 64, 400, 30);
    draw_text(renderer, "Press (-) to unlink your account locally.", 64, 500, 30);
    draw_text(renderer, "Press (HOME) to exit.", 64, 700, 30);

    SDL_RenderPresent(renderer);
}

bool new_graphics() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    get_user_information();

    const int SCREEN_WIDTH = 1920;
    const int SCREEN_HEIGHT = 1080;

    SDL_Window* window = SDL_CreateWindow("Wii U Account Swap", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (window == NULL) {
        return false;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        return false;
    }

    draw_menu_screen(renderer);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    bool is_running = true;

    while (is_running) {
        VPADStatus input;
        VPADReadError error;

        VPADRead(VPAD_CHAN_0, &input, 1, &error);

        if (input.trigger & VPAD_BUTTON_X) {
            is_running = false;
        }
    }

    SDL_Quit();

    return true;
}