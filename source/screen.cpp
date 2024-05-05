#include <string>

#include <coreinit/debug.h>
#include <coreinit/memory.h>
#include <coreinit/thread.h>
#include <nn/act.h>
#include <padscore/kpad.h>
#include <SDL2/SDL_ttf.h>
#include <vpad/input.h>

#include <fa-solid-900_ttf.h>
#include <ter-u32b_bdf.h>
#include "input.hpp"
#include "main.hpp"
#include "unlink.hpp"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;


void draw_background(int r, int g, int b, int a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);
}


void draw_rectangle(int x, int y, int w, int h, int r, int g, int b, int a) {
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderFillRect(renderer, &rect);
}


void draw_text(const char* text, int x, int y, int size, SDL_Color color = {255, 255, 255, 255}) {
    // We will fetch the system font from the shared data.
    void* font_data = nullptr;
    uint32_t font_size = 0;
    OSGetSharedData(OS_SHAREDDATATYPE_FONT_STANDARD, 0, &font_data, &font_size);

    TTF_Font* font = TTF_OpenFontRW(SDL_RWFromMem((void*)font_data, font_size), 1, size);
    if (font == NULL) {
        return;
    }

    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);
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

void draw_icon(const char* icon, int x, int y, int size, SDL_Color color = {255, 255, 255, 255}) {
    // fa-solid-900.ttf is a font file that contains icons.
    TTF_Font* font = TTF_OpenFontRW(SDL_RWFromMem((void*)fa_solid_900_ttf, fa_solid_900_ttf_size), 1, size);
    if (font == NULL) {
        return;
    }

    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, icon, color);
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


void draw_screen_bars() {
    draw_rectangle(0, 0, SCREEN_WIDTH, 90, 125, 0, 125, 255);
    draw_text("Wii U Account Swap", 64, 10, 50);
    draw_text(APP_VERSION, 64 + get_text_width("Wii U Account Swap", 50) + 16, 10, 50, {176, 176, 176, 255});
    draw_text("Nightkingale", SCREEN_WIDTH - 64 - get_text_width("Nightkingale", 50), 10, 50);

    draw_rectangle(0, 940, SCREEN_WIDTH, 140, 125, 0, 125, 255);
    draw_text("Current User: ", 64, 955, 40);
    draw_text(MII_NICKNAME.c_str(), 64 + get_text_width("Current User: ", 40), 955, 40, {176, 176, 176, 255});

    if (INKAY_EXISTS)
        draw_icon("\ue55c", 64 + get_text_width("Current User: ", 40) + get_text_width(MII_NICKNAME.c_str(), 40) + 16, 960, 40, {176, 176, 176, 255});
    else
        draw_icon("\ue560", 64 + get_text_width("Current User: ", 40) + get_text_width(MII_NICKNAME.c_str(), 40) + 16, 960, 40, {176, 176, 176, 255});

    draw_text(ACCOUNT_FILE.c_str(), 64, 1005, 40);

    draw_text("\ue07d Navigate", SCREEN_WIDTH - 42 - get_text_width("\ue07d Navigate", 50), 975, 50);
}


void draw_confirm_button() {
    draw_rectangle(0, 940, SCREEN_WIDTH, 140, 125, 0, 125, 255);
    draw_text("\ue000 Confirm", 64, 975, 50);
    draw_text("\ue001 Decline", SCREEN_WIDTH - 54 - get_text_width("\ue001 Decline", 50), 975, 50);
    SDL_RenderPresent(renderer);
}


void draw_menu_screen(int selected_menu_item) {
    draw_background(16, 16, 16, 255);
    draw_screen_bars();

    const char* menu_options[] = {
        "Switch to Nintendo Network ID",
        "Switch to Pretendo Network ID",
        "Backup Current Account",
        "Unlink Account Locally"
    };

    const char* menu_icons[] = {
        "\uf0ac",
        "\uf233",
        "\uf0c7",
        "\uf12d"
    };

    const int NUM_MENU_ITEMS = sizeof(menu_options) / sizeof(menu_options[0]);

    for (int item = 0; item < NUM_MENU_ITEMS; item++) {
        if (item != selected_menu_item) {
            draw_rectangle(0, 90 + item * 120, SCREEN_WIDTH, 120, 22, 22, 22, 255); // Gray border.
            draw_rectangle(5, 95 + item * 120, 1910, 110, 25, 25, 25, 255); // Gray rectangle.
        }
        draw_text(menu_options[item], 160, 120 + item * 120, 50);
        draw_icon(menu_icons[item], 64, 125 + item * 120, 50);
    }

    for (int item = 0; item < NUM_MENU_ITEMS; item++) {
        if (item == selected_menu_item) {
            draw_rectangle(0, 90 + item * 120, SCREEN_WIDTH, 120, 100, 100, 255, 255); // Blue border.
            draw_rectangle(5, 95 + item * 120, 1910, 110, 0, 0, 0, 255); // Black rectangle.
            draw_text("\ue000", SCREEN_WIDTH - 110, 115 + item * 120, 50, {100, 100, 255, 255});
        }
        draw_text(menu_options[item], 160, 120 + item * 120, 50);
        draw_icon(menu_icons[item], 64, 125 + item * 120, 50);
    }

    SDL_RenderPresent(renderer);
}


void draw_unlink_menu() {
    draw_background(16, 16, 16, 255);
    draw_screen_bars();

    draw_text("Unlinking: Please read the following and confirm!", 64, 160, 50);

    draw_text("This will unlink your Network ID from this user.", 64, 270, 50);
    draw_text("You can reattach this account to any user on this Wii U,", 64, 330, 50);
    draw_text("or attach a new account to this user.", 64, 390, 50);

    draw_text("However, this unlink will not take place on the server.", 64, 510, 50);
    draw_text("You won't be able to use this account on any other Wii U.", 64, 570, 50);
    
    draw_confirm_button();

    SDL_RenderPresent(renderer);
}


void draw_backup_menu() {
    draw_background(16, 16, 16, 255);
    draw_screen_bars();

    draw_text("Backup: Please read the following and confirm!", 64, 160, 50);

    draw_text("This will backup your current account.dat file.", 64, 270, 50);
    draw_text("The account.dat may contain sensitive personal", 64, 330, 50);
    draw_text("information, such as your e-mail address and encrypted", 64, 390, 50);
    draw_text("cached password (if you have chosen to save it).", 64, 450, 50);

    draw_text("Please do not share these backups with anyone else!", 64, 560, 50);

    draw_confirm_button();

    SDL_RenderPresent(renderer);
}


void draw_overwrite_menu(const char* backup_path) {
    draw_background(10, 10, 90, 255);
    draw_screen_bars();

    draw_text("Backup: Please read the following and confirm!", 64, 160, 50);

    draw_text("This will overwrite the existing backup file:", 64, 270, 50);
    draw_text(backup_path, 64, 330, 50);

    draw_text("Are you sure you want to overwrite this file?", 64, 440, 50);

    draw_confirm_button();

    SDL_RenderPresent(renderer);
}


void draw_error_menu(const char* error_message) {
    draw_background(90, 10, 10, 255);
    draw_screen_bars();

    draw_text("An exception has occurred!", 64, 160, 50);

    draw_text(error_message, 64, 270, 50);
    draw_text("You will return to the main menu.", 64, 330, 50);

    SDL_RenderPresent(renderer);
    OSSleepTicks(OSMillisecondsToTicks(5000));
}

void draw_success_menu(const char* type, bool inkay_configured = false) {
    draw_background(10, 60, 10, 255);
    draw_screen_bars();

    draw_text("The operation was successful!", 64, 160, 50);

    if (strcmp(type, "backup") == 0) {
        draw_text("Your account.dat file has been backed up.", 64, 270, 50);
        draw_text("You will return to the main menu.", 64, 330, 50);

    } else if (strcmp(type, "unlink") == 0) {
        draw_text("Your Network ID has been unlinked from this user.", 64, 270, 50);
        draw_text("The Wii U will now reboot.", 64, 330, 50);

    } else if (strcmp(type, "switch") == 0) {
        draw_text("Your account has been switched successfully.", 64, 270, 50);
        draw_text("The Wii U will now reboot.", 64, 330, 50);

        if (inkay_configured) {
            draw_text("Inkay was also configured automatically!", 64, 440, 50);
        }
    }

    SDL_RenderPresent(renderer);
    OSSleepTicks(OSMillisecondsToTicks(5000));
}