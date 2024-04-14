#include <iomanip>
#include <stdarg.h>
#include <stdio.h>
#include <string>

#include <coreinit/screen.h>
#include <coreinit/time.h>
#include <coreinit/thread.h>

#include "../include/main.hpp"
#include "../include/utils.hpp"


void printMainMenu() {
    OSScreenClearBufferEx(SCREEN_TV, 0x4A198500);
    OSScreenClearBufferEx(SCREEN_DRC, 0x4A198500);

    std::stringstream versionLine; // Create a stringstream to format the version line.
    versionLine << "Wii U Account Swap (" << VERSION_NUMBER << ")";
    int currentLength = versionLine.str().length();
    int numSpaces = 56 - currentLength - 11; // 11 is the length of " Nightkingale"
    for (int i = 0; i < numSpaces; i++) {
        versionLine << ' '; // Add spaces to format the version line.
    }
    versionLine << "Nightkingale";

    printOnScreen(0, versionLine.str().c_str());
    printOnScreen(1, "---------------------------------------------------------");
    
    printOnScreen(3, "Press (A) to switch to Nintendo Network ID.");
    printOnScreen(4, "Press (B) to switch to Pretendo Network ID.");
    printOnScreen(5, "Press (+) to backup your current account.");
    printOnScreen(6, "Press (-) to unlink your account locally.");

    printOnScreen(8, "Press (HOME) to exit.");
   
    printOnScreen(14, "---------------------------------------------------------");
    printOnScreen(15, "Current User: %s (%x)", MII_NICKNAME.c_str(), USER_ID);
    printOnScreen(16, "%s", ACCOUNT_FILE.c_str());

    OSScreenFlipBuffersEx(SCREEN_TV);
    OSScreenFlipBuffersEx(SCREEN_DRC);
}

void printUnlinkMenu() {
    OSScreenClearBufferEx(SCREEN_TV, 0x4A198500);
    OSScreenClearBufferEx(SCREEN_DRC, 0x4A198500);

    printOnScreen(0, "Unlinking: Please read the following and confirm!");
    printOnScreen(1, "---------------------------------------------------------");
    
    printOnScreen(3, "This will unlink your Network ID from this user.");
    printOnScreen(4, "You can reattach this account to any user on this Wii U,");
    printOnScreen(5, "or attach a new account to this user.");

    printOnScreen(7, "However, this unlink will not take place on the server.");
    printOnScreen(8, "You won't be able to use this account on any other Wii U.");

    printOnScreen(10, "Press (A) to confirm the unlink or (B) to cancel.");
    
    printOnScreen(14, "---------------------------------------------------------");
    printOnScreen(15, "Current User: %s (%x)", MII_NICKNAME.c_str(), USER_ID);
    printOnScreen(16, "%s", ACCOUNT_FILE.c_str());

    OSScreenFlipBuffersEx(SCREEN_TV);
    OSScreenFlipBuffersEx(SCREEN_DRC);
}

void printBackupMenu() {
    OSScreenClearBufferEx(SCREEN_TV, 0x4A198500);
    OSScreenClearBufferEx(SCREEN_DRC, 0x4A198500);

    printOnScreen(0, "Backup: Please read the following and confirm!");
    printOnScreen(1, "---------------------------------------------------------");
    
    printOnScreen(3, "This will backup your current account.dat file.");

    printOnScreen(5, "The account.dat may contain sensitive personal");
    printOnScreen(6, "information, such as your e-mail address and encrypted");
    printOnScreen(7, "cached password (if you have chosen to save it).");

    printOnScreen(9, "Please do not share these backups with anyone else!");

    printOnScreen(11, "Press (A) to confirm the backup or (B) to cancel.");
    
    printOnScreen(14, "---------------------------------------------------------");
    printOnScreen(15, "Current User: %s (%x)", MII_NICKNAME.c_str(), USER_ID);
    printOnScreen(16, "%s", ACCOUNT_FILE.c_str());

    OSScreenFlipBuffersEx(SCREEN_TV);
    OSScreenFlipBuffersEx(SCREEN_DRC);
}

void printOverwriteMenu(const char* backupPath) {
    OSScreenClearBufferEx(SCREEN_TV, 0x4A198500);
    OSScreenClearBufferEx(SCREEN_DRC, 0x4A198500);

    printOnScreen(0, "Backup: A backup file already exists!");
    printOnScreen(1, "---------------------------------------------------------");
    
    printOnScreen(3, "The backup file already exists!");
    printOnScreen(4, "%s", backupPath);
    printOnScreen(5, "Would you like to overwrite it?");

    printOnScreen(7, "Press (A) to overwrite the backup or (B) to cancel.");
    
    printOnScreen(14, "---------------------------------------------------------");
    printOnScreen(15, "Current User: %s (%x)", MII_NICKNAME.c_str(), USER_ID);
    printOnScreen(16, "%s", ACCOUNT_FILE.c_str());

    OSScreenFlipBuffersEx(SCREEN_TV);
    OSScreenFlipBuffersEx(SCREEN_DRC);
}