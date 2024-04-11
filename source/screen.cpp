#include <stdarg.h>
#include <stdio.h>
#include <string>

#include <coreinit/screen.h>
#include <coreinit/time.h>
#include <coreinit/thread.h>

#include "../include/global.h"


void printOnScreen(int line, const char* format, ...) {
    char buffer[256];
    va_list args;
    // Format the string into a buffer.
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    // Print the string to the screen.
    OSScreenPutFontEx(SCREEN_TV, 0, line, buffer);
    OSScreenPutFontEx(SCREEN_DRC, 0, line, buffer);
}

void printMainMenu() {
    OSScreenClearBufferEx(SCREEN_TV, 0x4A198500);
    OSScreenClearBufferEx(SCREEN_DRC, 0x4A198500);

    printOnScreen(0, "Wii U Account Swap (v1.0.0)");
    printOnScreen(1, "Created by Nightkingale");
    printOnScreen(2, "----------------------------------------------------------");
    printOnScreen(3, "Press (A) to switch to Nintendo Network ID.");
    printOnScreen(4, "Press (B) to switch to Pretendo Network ID.");
    printOnScreen(5, "Press (-) to unlink your account locally.");
    printOnScreen(6, "Press (HOME) to exit.");
    printOnScreen(7, "----------------------------------------------------------");
    printOnScreen(8, "Current User: %s (%x)", MII_NICKNAME.c_str(), USER_ID);
    printOnScreen(9, "%s", ACCOUNT_FILE.c_str());

    OSScreenFlipBuffersEx(SCREEN_TV);
    OSScreenFlipBuffersEx(SCREEN_DRC);
}

void printWarningMenu() {
    OSScreenClearBufferEx(SCREEN_TV, 0x4A198500);
    OSScreenClearBufferEx(SCREEN_DRC, 0x4A198500);

    printOnScreen(0, "Unlinking: Please read the following and confirm!");
    printOnScreen(1, "----------------------------------------------------------");
    printOnScreen(2, "This will unlink your Network ID from this user.");
    printOnScreen(3, "You can reattach this account to any user on this Wii U,");
    printOnScreen(4, "or attach a new account to this user.");

    printOnScreen(6, "However, this unlink will not take place on the server.");
    printOnScreen(7, "You won't be able to use this account on any other Wii U.");
    printOnScreen(8, "----------------------------------------------------------");
    printOnScreen(9, "Press (A) to confirm the unlink or (B) to cancel.");

    OSScreenFlipBuffersEx(SCREEN_TV);
    OSScreenFlipBuffersEx(SCREEN_DRC);
}