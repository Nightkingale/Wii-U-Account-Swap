#include <coreinit/launch.h>
#include <mocha/mocha.h>
#include <sysapp/launch.h>

#include "start_screen.hpp"

#include "backup.hpp"
#include "easter_egg.hpp"
#include "input.hpp"
#include "main.hpp"
#include "nintendo_glyphs.hpp"
#include "sub_screens.hpp"
#include "swap.hpp"
#include "unlink.hpp"
#include "utils.hpp"


int selected_option = 0;
const int NUM_OPTIONS = 4;
screen current_screen = start_screen;


bool
swap_account_action(const char* account_backup, account account_type)
{
    if (swap_account(account_backup, account_type)) {
        OSLaunchTitlel(OS_TITLE_ID_REBOOT, 0);
        return true;
    }
    current_screen = start_screen;
    return false;
}


bool
backup_account_action()
{
    OSEnableHomeButtonMenu(0);
    draw_backup_menu();
    int button = read_input();

    if (button & VPAD_BUTTON_A) {
        backup_account();
        current_screen = start_screen;
        return true;
    } else if (button & VPAD_BUTTON_B)
    {
        current_screen = start_screen;
        return true;
    }
    return false;
}


bool
unlink_account_action()
{
    OSEnableHomeButtonMenu(0);
    draw_unlink_menu();
    int button = read_input();

    if (button & VPAD_BUTTON_A) {
        if (unlink_account()) {
            OSLaunchTitlel(OS_TITLE_ID_REBOOT, 0);
            return true;
        }
        current_screen = start_screen;
        return false;
    }
    else if (button & VPAD_BUTTON_B)
    {
        current_screen = start_screen;
        return true;
    }
    return false;
}


void
draw_start_screen(int selected_menu_item)
{
    draw_background(16, 16, 16, 255);
    draw_screen_bars(); // This gives us the top and bottom bars.

    struct menu_item {
        const char* icon;
        const char* option;
    };

    const menu_item choices[] = {
        {"\uF0AC", "Swap to Nintendo Network ID"}, // Network icon.
        {"\uF233", "Swap to Pretendo Network ID"}, // Server icon.
        {"\uF0C7", "Backup the account.dat File"}, // Save icon.
        {"\uF12D", "Unlink the account.dat File"}  // Unlink icon.
    };

    const int NUM_MENU_ITEMS = sizeof(choices) / sizeof(choices[0]);

    for (int item = 0; item < NUM_MENU_ITEMS; item++) {
        bool is_selected = item == selected_menu_item;
        draw_rectangle(0, 90 + item * 120, SCREEN_WIDTH, 120, \
            is_selected ? 100 : 22, is_selected ? 100 : 22, is_selected ? 255 : 22, 255);
        draw_rectangle(5, 95 + item * 120, 1910, 110, \
            is_selected ? 0 : 25, is_selected ? 0 : 25, is_selected ? 0 : 25, 255);
        draw_text(choices[item].option, 160, 120 + item * 120, 50);

        SDL_Color icon_color = {255, 255, 255, 255};
        if (item == 0) {
            icon_color = {255, 150, 0, 255}; // Orange icon.
        } else if (item == 1) {
            icon_color = {255, 50, 255, 255}; // Purple icon.
        }
        draw_icon(choices[item].icon, 64, 125 + item * 120, 50, icon_color);

        if (is_selected) {
            draw_text(NIN_GLYPH_BTN_A, SCREEN_WIDTH - 64 - get_text_size(NIN_GLYPH_BTN_A, 50),
                115 + item * 120, 50, {100, 100, 255, 255});
        }
    }

    SDL_RenderPresent(renderer);
}


void
process_start_screen()
{
    draw_start_screen(selected_option); // Draw the start screen.
    int button = read_input(); // Watch the controllers for input.
    OSEnableHomeButtonMenu(1);

    if (button & VPAD_BUTTON_UP) {
        selected_option--;
        if (selected_option < 0)
            selected_option = NUM_OPTIONS - 1;
    } else if (button & VPAD_BUTTON_DOWN) {
        selected_option++;
        if (selected_option >= NUM_OPTIONS)
            selected_option = 0;
    } else if (button & VPAD_BUTTON_A) {
        switch (selected_option) {
            case 0:
                current_screen = swap_screen;
                // swap_account_action(NNID_BACKUP.c_str(), "Nintendo");
                break;
            case 1:
                current_screen = swap_screen;
                // swap_account_action(PNID_BACKUP.c_str(), "Pretendo");
                break;
            case 2:
                current_screen = backup_screen;
                // backup_account_action();
                break;
            case 3:
                current_screen = unlink_screen;
                // unlink_account_action();
                break;
        }
    } else if (button & VPAD_BUTTON_SYNC) {
        play_easter_egg();
    }
}


void
process_screens()
{
    switch(current_screen)
    {
        case start_screen:
            process_start_screen();
            break;
        case unlink_screen:
            unlink_account_action();
            break;
        case swap_screen:
            if(selected_option == 0)
                swap_account_action(NNID_BACKUP.c_str(), account::nintendo_network_id);
            else
                swap_account_action(PNID_BACKUP.c_str(), account::pretendo_network_id);
            break;
        case backup_screen:
            backup_account_action();
            break;
        case overwrite_screen:
            break;
    }
}
