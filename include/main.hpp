#ifndef MAIN_HPP
#define MAIN_HPP

#include <string>


const int BUFFER_SIZE = 0x2000; // The buffer size allocated for reading and writing files.

extern unsigned int USER_ID; // The current user persistant ID.
extern std::string NNID_BACKUP; // The backup path to the Nintendo Network ID account.dat.
extern std::string PNID_BACKUP; // The backup path to the Pretendo Network ID account.dat.
extern std::string MII_NICKNAME; // The current user's Mii nickname.
extern std::string ACCOUNT_FILE; // The path to the current account.dat.
extern std::string INKAY_CONFIG; // The path to the Inkay configuration file.


void get_user_information();
void deinitialize();


#endif