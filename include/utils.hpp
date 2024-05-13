#ifndef UTILS_HPP
#define UTILS_HPP

#include <SDL2/SDL.h>


const int SCREEN_WIDTH = 1920; // 1920px (HD)
const int SCREEN_HEIGHT = 1080; // 1080px (HD) 


void draw_background(int r, int g, int b, int a);
void draw_rectangle(int x, int y, int w, int h, int r, int g, int b, int a);
void draw_text(const char* text, int x, int y, int size, SDL_Color color = {255, 255, 255, 255});
void draw_icon(const char* icon, int x, int y, int size, SDL_Color color = {255, 255, 255, 255});
int get_text_size(const char* text, int size, bool get_height = false);
void close_fonts();
void draw_screen_bars(bool show_confirm = false, bool show_controls = true);


#endif