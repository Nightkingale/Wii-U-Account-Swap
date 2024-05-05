#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include <coreinit/debug.h>
#include <coreinit/memory.h>
#include <nn/act.h>
#include <vpad/input.h>

#include <fa-solid-900_ttf.h>
#include <ter-u32b_bdf.h>
#include "input.hpp"
#include "main.hpp"
#include "unlink.hpp"



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

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
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

void draw_menu_screen(SDL_Renderer* renderer, int selected_menu_item) {
    draw_background(renderer, 16, 16, 16, 255);

    // Top Bar
    draw_rectangle(renderer, 0, 0, 1920, 90, 153, 0, 153, 255);
    draw_text(renderer, "Wii U Account Swap", 64, 10, 50);
    draw_text(renderer, APP_VERSION, 64 + get_text_width("Wii U Account Swap", 50) + 16, 10, 50, {176, 176, 176, 255});
    draw_text(renderer, "Nightkingale", 1920 - 64 - get_text_width("Nightkingale", 50), 10, 50);

    // Bottom Bar
    draw_rectangle(renderer, 0, 940, 1920, 140, 153, 0, 153, 255);
    draw_text(renderer, "Current User: ", 64, 955, 40);
    draw_text(renderer, MII_NICKNAME.c_str(), 64 + get_text_width("Current User: ", 40), 955, 40, {176, 176, 176, 255});
    draw_text(renderer, ACCOUNT_FILE.c_str(), 64, 1005, 40);

    // Menu options
    const char* menu_options[] = {
        "Switch to Nintendo Network ID",
        "Switch to Pretendo Network ID",
        "Backup the Current User",
        "Unlink the Network ID"
    };
    const int NUM_MENU_ITEMS = sizeof(menu_options) / sizeof(menu_options[0]);

    for (int i = 0; i < NUM_MENU_ITEMS; i++) {
        if (i == selected_menu_item) {
            // Draw black rectangle with blue border behind selected option
            draw_rectangle(renderer, 64, 135 + i * 120, 1797, 110, 100, 100, 255, 255); // blue border
            draw_rectangle(renderer, 69, 140 + i * 120, 1787, 100, 0, 0, 0, 255); // black rectangle
        }
        draw_text(renderer, menu_options[i], 93, 160 + i * 120, 50);
    }

    SDL_RenderPresent(renderer);
}

// Draw warning screen for unlinking account
void draw_unlink_menu(SDL_Renderer* renderer) {
    draw_background(renderer, 16, 16, 16, 255);

    // Top Bar
    draw_rectangle(renderer, 0, 0, 1920, 90, 153, 0, 153, 255);
    draw_text(renderer, "Wii U Account Swap", 64, 10, 50);
    draw_text(renderer, APP_VERSION, 64 + get_text_width("Wii U Account Swap", 50) + 16, 10, 50, {176, 176, 176, 255});
    draw_text(renderer, "Nightkingale", 1920 - 64 - get_text_width("Nightkingale", 50), 10, 50);

    // Bottom Bar
    draw_rectangle(renderer, 0, 940, 1920, 140, 153, 0, 153, 255);
    draw_text(renderer, "Current User: ", 64, 955, 40);
    draw_text(renderer, MII_NICKNAME.c_str(), 64 + get_text_width("Current User: ", 40), 955, 40, {176, 176, 176, 255});
    draw_text(renderer, ACCOUNT_FILE.c_str(), 64, 1005, 40);

    // Warning text
    draw_text(renderer, "Unlinking: Please read the following and confirm!", 64, 160, 50);

    draw_text(renderer, "This will unlink your Network ID from this user.", 64, 270, 50);
    draw_text(renderer, "You can reattach this account to any user on this Wii U,", 64, 330, 50);
    draw_text(renderer, "or attach a new account to this user.", 64, 390, 50);
    draw_text(renderer, "However, this unlink will not take place on the server.", 64, 450, 50);
    draw_text(renderer, "You won't be able to use this account on any other Wii U.", 64, 510, 50);
    
    // Draw a confirm button with border
    draw_rectangle(renderer, 64, 650, 896, 100, 100, 100, 255, 255); // blue border
    draw_rectangle(renderer, 69, 655, 886, 90, 0, 0, 0, 255); // black rectangle
    draw_text(renderer, "Confirm Unlink", 93, 670, 50);

    SDL_RenderPresent(renderer);
}

// Draw backup overwrite warning screen
void draw_backup_menu(SDL_Renderer* renderer) {
    draw_background(renderer, 16, 16, 16, 255);

    // Top Bar
    draw_rectangle(renderer, 0, 0, 1920, 90, 153, 0, 153, 255);
    draw_text(renderer, "Wii U Account Swap", 64, 10, 50);
    draw_text(renderer, APP_VERSION, 64 + get_text_width("Wii U Account Swap", 50) + 16, 10, 50, {176, 176, 176, 255});
    draw_text(renderer, "Nightkingale", 1920 - 64 - get_text_width("Nightkingale", 50), 10, 50);

    // Bottom Bar
    draw_rectangle(renderer, 0, 940, 1920, 140, 153, 0, 153, 255);
    draw_text(renderer, "Current User: ", 64, 955, 40);
    draw_text(renderer, MII_NICKNAME.c_str(), 64 + get_text_width("Current User: ", 40), 955, 40, {176, 176, 176, 255});
    draw_text(renderer, ACCOUNT_FILE.c_str(), 64, 1005, 40);

    // Warning text
    draw_text(renderer, "Backup: Please read the following and confirm!", 64, 160, 50);

    draw_text(renderer, "This will backup your current account.dat file.", 64, 270, 50);
    draw_text(renderer, "The account.dat may contain sensitive personal", 64, 330, 50);
    draw_text(renderer, "information, such as your e-mail address and encrypted", 64, 390, 50);
    draw_text(renderer, "cached password (if you have chosen to save it).", 64, 450, 50);
    draw_text(renderer, "Please do not share these backups with anyone else!", 64, 510, 50);

    // Draw a confirm button with border
    draw_rectangle(renderer, 64, 650, 896, 100, 100, 100, 255, 255); // blue border
    draw_rectangle(renderer, 69, 655, 886, 90, 0, 0, 0, 255); // black rectangle
    draw_text(renderer, "Confirm Backup", 93, 670, 50);

    SDL_RenderPresent(renderer);
}

// Draw overwrite menu
void draw_overwrite_menu(SDL_Renderer* renderer, const char* backup_path) {
    draw_background(renderer, 16, 16, 16, 255);

    // Top Bar
    draw_rectangle(renderer, 0, 0, 1920, 90, 153, 0, 153, 255);
    draw_text(renderer, "Wii U Account Swap", 64, 10, 50);
    draw_text(renderer, APP_VERSION, 64 + get_text_width("Wii U Account Swap", 50) + 16, 10, 50, {176, 176, 176, 255});
    draw_text(renderer, "Nightkingale", 1920 - 64 - get_text_width("Nightkingale", 50), 10, 50);

    // Bottom Bar
    draw_rectangle(renderer, 0, 940, 1920, 140, 153, 0, 153, 255);
    draw_text(renderer, "Current User: ", 64, 955, 40);
    draw_text(renderer, MII_NICKNAME.c_str(), 64 + get_text_width("Current User: ", 40), 955, 40, {176, 176, 176, 255});
    draw_text(renderer, ACCOUNT_FILE.c_str(), 64, 1005, 40);

    // Warning text
    draw_text(renderer, "Backup: Please read the following and confirm!", 64, 160, 50);

    draw_text(renderer, "This will overwrite the existing backup file:", 64, 270, 50);
    draw_text(renderer, backup_path, 64, 330, 50);
    draw_text(renderer, "Are you sure you want to overwrite this file?", 64, 390, 50);

    // Draw a confirm button with border
    draw_rectangle(renderer, 64, 650, 896, 100, 100, 100, 255, 255); // blue border
    draw_rectangle(renderer, 69, 655, 886, 90, 0, 0, 0, 255); // black rectangle
    draw_text(renderer, "Confirm Overwrite", 93, 670, 50);

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

    bool is_running = true;
    int selected_option = 0;
    const int NUM_OPTIONS = 4;

    while (is_running) {
        int button = read_input();

        if (button == VPAD_BUTTON_UP) {
            selected_option--;
            if (selected_option < 0) {
                selected_option = NUM_OPTIONS - 1;
            }
        } else if (button == VPAD_BUTTON_DOWN) {
            selected_option++;
            if (selected_option >= NUM_OPTIONS) {
                selected_option = 0;
            }
        } else if (button == VPAD_BUTTON_A) {
            switch (selected_option) {
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    while (is_running) {
                        draw_backup_menu(renderer);
                        button = read_input();

                        if (button == VPAD_BUTTON_A) {
                            break;
                        } else if (button == VPAD_BUTTON_B) {
                            break;
                        }
                    }
                case 3:
                    while (is_running) {
                        draw_unlink_menu(renderer);
                        button = read_input();

                        if (button == VPAD_BUTTON_A) {
                            break;
                        } else if (button == VPAD_BUTTON_B) {
                            break;
                        }
                    }
                    break;
            }
        } else if (button == VPAD_BUTTON_X) {
            is_running = false;
        }

        draw_menu_screen(renderer, selected_option);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return true;
}