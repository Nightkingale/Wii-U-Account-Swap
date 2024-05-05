#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <SDL2/SDL.h>


void draw_menu_screen(int selected_menu_item);
void draw_unlink_menu();
void draw_backup_menu();
void draw_overwrite_menu(const char* backup_path);


#endif