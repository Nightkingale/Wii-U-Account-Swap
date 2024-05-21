#include <string>

#include <SDL2/SDL_ttf.h>

#include "sub_screens.hpp"

#include "input.hpp"
#include "main.hpp"
#include "nintendo_glyphs.hpp"
#include "unlink.hpp"
#include "utils.hpp"


void
draw_unlink_menu()
{
    draw_background(16, 16, 16, 255);
    draw_screen_bars(true);

    draw_text("Before unlinking, please read the following notice!", 64, 120, 50);

    draw_text("This will unlink your Network ID from this user.", 64, 230, 50);
    draw_text("You can reattach this account to any user on this Wii U,", 64, 290, 50);
    draw_text("or attach a new account to this user.", 64, 350, 50);

    draw_text("However, this unlink will not take place on the server.", 64, 460, 50);
    draw_text("You won't be able to use this account on any other Wii U.", 64, 520, 50);

    draw_text("You must confirm to continue the process.", 64, 840, 50);

    SDL_RenderPresent(renderer);
}


void
draw_backup_menu()
{
    draw_background(16, 16, 16, 255);
    draw_screen_bars(true);

    draw_text("Before backing up, please read the following notice!", 64, 120, 50);

    draw_text("This will backup your current account.dat file.", 64, 230, 50);
    draw_text("The account.dat may contain sensitive personal", 64, 340, 50);
    draw_text("information, such as your e-mail address and encrypted", 64, 400, 50);
    draw_text("cached password (if you have chosen to save it).", 64, 460, 50);

    draw_text("Please do not share these backups with anyone else!", 64, 570, 50);

    draw_text("You must confirm to continue the process.", 64, 840, 50);

    SDL_RenderPresent(renderer);
}


void
draw_overwrite_menu(const char* backup_path)
{
    draw_background(10, 10, 60, 255); // Blue background.
    draw_screen_bars(true);

    draw_text("Before backing up, please read the following notice!", 64, 120, 50);

    draw_text("The backup file already exists!", 64, 230, 50);
    draw_text(backup_path, 64, 290, 50);

    draw_text("Would you like to overwrite it?", 64, 400, 50);

    draw_text("You must confirm to continue the process.", 64, 840, 50);

    SDL_RenderPresent(renderer);
}


void
draw_error_menu(const char* error_message)
{
    draw_background(60, 10, 10, 255); // Red background.
    draw_screen_bars(false, false);

    draw_text("An exception has occurred!", 64, 120, 50);

    draw_text(error_message, 64, 230, 50);
    draw_text("You will return to the main menu.", 64, 290, 50);

    draw_text("If you believe this to be a bug, please report it.", 64, 840, 50);

    SDL_RenderPresent(renderer);
    SDL_Delay(5000);
}


void
draw_success_menu(success type, bool inkay_configured)
{
    draw_background(10, 60, 10, 255); // Green background.
    draw_screen_bars(false, false);

    draw_text("The operation was successful!", 64, 120, 50);

    switch (type) {
        case success::backup:
            draw_text("The account.dat was backed up successfully!", 64, 230, 50);
            draw_text("The main menu will appear in 5 seconds...", 64, 290, 50);
            break;

        case success::unlink:
            draw_text("The account.dat was unlinked successfully!", 64, 230, 50);
            draw_text("Your console will restart in 5 seconds...", 64, 290, 50);
            break;

        case success::swap:
            draw_text("The account.dat was restored successfully!", 64, 230, 50);
            draw_text("Your console will restart in 5 seconds...", 64, 290, 50);

            if (inkay_configured) {
                draw_text("Please note that Inkay was configured automatically.", 64, 840, 50);
            }
            break;
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(5000);
}