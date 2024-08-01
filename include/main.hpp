#ifndef MAIN_HPP
#define MAIN_HPP

#include <string>

#include <SDL2/SDL.h>


const int BUFFER_SIZE = 0x2000; // The buffer size allocated for reading and writing files.

extern bool INKAY_EXISTS; // Whether the Inkay plugin config exists.
extern std::string NNID_BACKUP; // The backup path to the Nintendo Network ID account.dat.
extern std::string PNID_BACKUP; // The backup path to the Pretendo Network ID account.dat.
extern std::string MII_NICKNAME; // The current user's Mii nickname.
extern std::string PERSISTENT_ID; // The current user persistant ID.
extern std::string ACCOUNT_ID; // The current user's account name.
extern std::string ACCOUNT_FILE; // The path to the current account.dat.
extern std::string INKAY_CONFIG; // The path to the Inkay configuration file.

extern SDL_Window* window; // Global window variable.
extern SDL_Renderer* renderer; // Global renderer variable;


void get_user_information();
void deinitialize();


#endif