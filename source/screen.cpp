#include <stdarg.h>
#include <stdio.h>
#include <string>

#include <coreinit/screen.h>
#include <coreinit/time.h>
#include <coreinit/thread.h>

#include "../include/global.h"


void printOnScreen(int line, const char* format, ...)
{
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

void printMainMenu()
{
    OSScreenClearBufferEx(SCREEN_TV, 0x4A198500);
    OSScreenClearBufferEx(SCREEN_DRC, 0x4A198500);

    printOnScreen(0, "Wii U Account Swap (v1.0.0)");
    printOnScreen(1, "Created by Nightkingale");
    printOnScreen(2, "----------------------------------------------------------");
    printOnScreen(3, "Press A to switch to Nintendo Network ID.");
    printOnScreen(4, "Press B to switch to Pretendo Network ID.");
    printOnScreen(5, "Press X to unlink your account locally.");
    printOnScreen(6, "Press HOME to exit.");
    printOnScreen(7, "----------------------------------------------------------");
    printOnScreen(8, "Current User: %s (%x)", MII_NICKNAME.c_str(), USER_ID);
    printOnScreen(9, "%s", ACCOUNT_FILE.c_str());

    OSScreenFlipBuffersEx(SCREEN_TV);
    OSScreenFlipBuffersEx(SCREEN_DRC);
}

void printWarningScreen()
{
    OSScreenClearBufferEx(SCREEN_TV, 0x4A198500);
    OSScreenClearBufferEx(SCREEN_DRC, 0x4A198500);

    printOnScreen(0, "Warning: This will unlink your account locally!");
    printOnScreen(1, "This will not unlink your account from the server!");
    printOnScreen(2, "----------------------------------------------------------");
    printOnScreen(3, "Your user can have a new account reattached!");
    printOnScreen(4, "You will be able to login again on this Wii U as normal.");
    printOnScreen(5, "However, you will NOT be able to login to another Wii U!");
    printOnScreen(6, "----------------------------------------------------------");
    printOnScreen(7, "Press A to confirm the unlink or B to cancel.");

    OSScreenFlipBuffersEx(SCREEN_TV);
    OSScreenFlipBuffersEx(SCREEN_DRC);
}