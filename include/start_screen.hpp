#ifndef START_SCREEN_HPP
#define START_SCREEN_HPP
enum Screens
{
    Start,
    Unlink,
    Backup,
    Swap,
    Overwrite
};

extern Screens CurrentScreen;

void draw_start_screen(int selected_menu_item);
void process_start_screen();
void process_screens();

#endif