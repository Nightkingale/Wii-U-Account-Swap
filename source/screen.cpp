#include <string>

#include <SDL2/SDL_ttf.h>

#include "input.hpp"
#include "main.hpp"
#include "unlink.hpp"
#include "video.hpp"


void draw_menu_screen(int selected_menu_item) {
    draw_background(16, 16, 16, 255);
    draw_screen_bars(); // This gives us the top and bottom bars.

    const char* menu_options[] = {
        "Switch to Nintendo Network ID",
        "Switch to Pretendo Network ID",
        "Backup Current Account",
        "Unlink Account Locally"
    }; // Menu options.

    const char* menu_icons[] = {
        "\uf0ac",
        "\uf233",
        "\uf0c7",
        "\uf12d"
    }; // Font Awesome icons.

    const int NUM_MENU_ITEMS = sizeof(menu_options) / sizeof(menu_options[0]); // Number of menu items.

    // Draw the unselected menu items.
    for (int item = 0; item < NUM_MENU_ITEMS; item++) {
        if (item != selected_menu_item) {
            draw_rectangle(0, 90 + item * 120, SCREEN_WIDTH, 120, 22, 22, 22, 255); // Gray border.
            draw_rectangle(5, 95 + item * 120, 1910, 110, 25, 25, 25, 255); // Gray rectangle.
        }
        draw_text(menu_options[item], 160, 120 + item * 120, 50);
        draw_icon(menu_icons[item], 64, 125 + item * 120, 50);
    }

    // Draw the selected menu item.
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

    draw_text("Unlinking: Please read the following and confirm!", 64, 120, 50);

    draw_text("This will unlink your Network ID from this user.", 64, 230, 50);
    draw_text("You can reattach this account to any user on this Wii U,", 64, 290, 50);
    draw_text("or attach a new account to this user.", 64, 350, 50);

    draw_text("However, this unlink will not take place on the server.", 64, 460, 50);
    draw_text("You won't be able to use this account on any other Wii U.", 64, 520, 50);
    
    draw_confirm_button();

    SDL_RenderPresent(renderer);
}


void draw_backup_menu() {
    draw_background(16, 16, 16, 255);
    draw_screen_bars();

    draw_text("Backup: Please read the following and confirm!", 64, 120, 50);

    draw_text("This will backup your current account.dat file.", 64, 230, 50);
    draw_text("The account.dat may contain sensitive personal", 64, 340, 50);
    draw_text("information, such as your e-mail address and encrypted", 64, 400, 50);
    draw_text("cached password (if you have chosen to save it).", 64, 460, 50);

    draw_text("Please do not share these backups with anyone else!", 64, 570, 50);

    draw_confirm_button();

    SDL_RenderPresent(renderer);
}


void draw_overwrite_menu(const char* backup_path) {
    draw_background(10, 10, 60, 255); // Blue background.
    draw_screen_bars();

    draw_text("Backup: Please read the following and confirm!", 64, 120, 50);

    draw_text("The backup file already exists!", 64, 230, 50);
    draw_text(backup_path, 64, 290, 50);

    draw_text("Would you like to overwrite it?", 64, 400, 50);

    draw_confirm_button();

    SDL_RenderPresent(renderer);
}


void draw_error_menu(const char* error_message) {
    draw_background(60, 10, 10, 255); // Red background.
    draw_screen_bars();

    draw_text("An exception has occurred!", 64, 120, 50);

    draw_text(error_message, 64, 230, 50);
    draw_text("You will return to the main menu.", 64, 290, 50);

    SDL_RenderPresent(renderer);
    SDL_Delay(5000);
}

void draw_success_menu(const char* type, bool inkay_configured = false) {
    draw_background(10, 60, 10, 255); // Green background.
    draw_screen_bars();

    draw_text("The operation was successful!", 64, 120, 50);

    if (strcmp(type, "backup") == 0) {
        draw_text("Your account.dat file has been backed up.", 64, 230, 50);
        draw_text("You will return to the main menu.", 64, 290, 50);

    } else if (strcmp(type, "unlink") == 0) {
        draw_text("Your Network ID has been unlinked from this user.", 64, 230, 50);
        draw_text("The Wii U will now reboot.", 64, 290, 50);

    } else if (strcmp(type, "switch") == 0) {
        draw_text("Your account has been switched successfully.", 64, 230, 50);
        draw_text("The Wii U will now reboot.", 64, 290, 50);

        if (inkay_configured) {
            draw_text("Inkay was also configured automatically!", 64, 400, 50);
        }
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(5000);
}