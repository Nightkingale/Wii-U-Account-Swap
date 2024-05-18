#ifndef START_SCREEN_HPP
#define START_SCREEN_HPP


enum screens
{
    START_SCREEN,
    UNLINK_SCREEN,
    BACKUP_SCREEN,
    SWAP_SCREEN,
    OVERWRITE_SCREEN
};


extern screens current_screen;


void draw_start_screen(int selected_menu_item);
void process_start_screen();
void process_screens();

#endif