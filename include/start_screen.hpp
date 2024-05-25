#ifndef START_SCREEN_HPP
#define START_SCREEN_HPP


enum screen
{
    start_screen,
    unlink_screen,
    backup_screen,
    swap_screen,
    overwrite_screen
};


extern screen current_screen;


void draw_start_screen(int selected_menu_item);
void process_start_screen();
void process_screens();


#endif