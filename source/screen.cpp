#include <iomanip>
#include <stdarg.h>
#include <stdio.h>
#include <string>

#include <coreinit/screen.h>
#include <coreinit/time.h>
#include <coreinit/thread.h>

#include "main.hpp"
#include "utils.hpp"


void print_main_menu() {
    OSScreenClearBufferEx(SCREEN_TV, 0x4A198500);
    OSScreenClearBufferEx(SCREEN_DRC, 0x4A198500);

    std::stringstream versionLine; // Create a stringstream to format the version line.
    versionLine << "Wii U Account Swap (" << APP_VERSION << ")";
    int currentLength = versionLine.str().length();
    int numSpaces = 56 - currentLength - 11; // 11 is the length of " Nightkingale"
    for (int i = 0; i < numSpaces; i++) {
        versionLine << ' '; // Add spaces to format the version line.
    }
    versionLine << "Nightkingale";

    print_on_screen(0, versionLine.str().c_str());
    print_on_screen(1, "---------------------------------------------------------");
    
    print_on_screen(3, "Press (A) to switch to Nintendo Network ID.");
    print_on_screen(4, "Press (B) to switch to Pretendo Network ID.");
    print_on_screen(5, "Press (+) to backup your current account.");
    print_on_screen(6, "Press (-) to unlink your account locally.");

    print_on_screen(8, "Press (HOME) to exit.");
   
    print_on_screen(14, "---------------------------------------------------------");
    print_on_screen(15, "Current User: %s (%x)", MII_NICKNAME.c_str(), USER_ID);
    print_on_screen(16, "%s", ACCOUNT_FILE.c_str());

    OSScreenFlipBuffersEx(SCREEN_TV);
    OSScreenFlipBuffersEx(SCREEN_DRC);
}


void print_unlink_menu() {
    OSScreenClearBufferEx(SCREEN_TV, 0x4A198500);
    OSScreenClearBufferEx(SCREEN_DRC, 0x4A198500);

    print_on_screen(0, "Unlinking: Please read the following and confirm!");
    print_on_screen(1, "---------------------------------------------------------");
    
    print_on_screen(3, "This will unlink your Network ID from this user.");
    print_on_screen(4, "You can reattach this account to any user on this Wii U,");
    print_on_screen(5, "or attach a new account to this user.");

    print_on_screen(7, "However, this unlink will not take place on the server.");
    print_on_screen(8, "You won't be able to use this account on any other Wii U.");

    print_on_screen(10, "Press (A) to confirm the unlink or (B) to cancel.");
    
    print_on_screen(14, "---------------------------------------------------------");
    print_on_screen(15, "Current User: %s (%x)", MII_NICKNAME.c_str(), USER_ID);
    print_on_screen(16, "%s", ACCOUNT_FILE.c_str());

    OSScreenFlipBuffersEx(SCREEN_TV);
    OSScreenFlipBuffersEx(SCREEN_DRC);
}


void print_backup_menu() {
    OSScreenClearBufferEx(SCREEN_TV, 0x4A198500);
    OSScreenClearBufferEx(SCREEN_DRC, 0x4A198500);

    print_on_screen(0, "Backup: Please read the following and confirm!");
    print_on_screen(1, "---------------------------------------------------------");
    
    print_on_screen(3, "This will backup your current account.dat file.");

    print_on_screen(5, "The account.dat may contain sensitive personal");
    print_on_screen(6, "information, such as your e-mail address and encrypted");
    print_on_screen(7, "cached password (if you have chosen to save it).");

    print_on_screen(9, "Please do not share these backups with anyone else!");

    print_on_screen(11, "Press (A) to confirm the backup or (B) to cancel.");
    
    print_on_screen(14, "---------------------------------------------------------");
    print_on_screen(15, "Current User: %s (%x)", MII_NICKNAME.c_str(), USER_ID);
    print_on_screen(16, "%s", ACCOUNT_FILE.c_str());

    OSScreenFlipBuffersEx(SCREEN_TV);
    OSScreenFlipBuffersEx(SCREEN_DRC);
}


void print_overwrite_menu(const char* backup_path) {
    OSScreenClearBufferEx(SCREEN_TV, 0x4A198500);
    OSScreenClearBufferEx(SCREEN_DRC, 0x4A198500);

    print_on_screen(0, "Backup: A backup file already exists!");
    print_on_screen(1, "---------------------------------------------------------");
    
    print_on_screen(3, "The backup file already exists!");
    print_on_screen(4, "%s", backup_path);
    print_on_screen(5, "Would you like to overwrite it?");

    print_on_screen(7, "Press (A) to overwrite the backup or (B) to cancel.");
    
    print_on_screen(14, "---------------------------------------------------------");
    print_on_screen(15, "Current User: %s (%x)", MII_NICKNAME.c_str(), USER_ID);
    print_on_screen(16, "%s", ACCOUNT_FILE.c_str());

    OSScreenFlipBuffersEx(SCREEN_TV);
    OSScreenFlipBuffersEx(SCREEN_DRC);
}