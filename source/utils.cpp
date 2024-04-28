#include <stdarg.h>
#include <stdio.h>
#include <string>

#include <coreinit/screen.h>
#include <coreinit/time.h>
#include <coreinit/thread.h>
#include <mocha/mocha.h>
#include <nn/act.h>
#include <whb/log_console.h>
#include <whb/log.h>
#include <whb/proc.h>

#include "../include/main.hpp"


void print_on_screen(int line, const char* format, ...) {
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


void deinitialize() {
    // This prevents hangs when called twice.
    static bool isDeinitialized = false;

    if (!isDeinitialized) {
        nn::act::Finalize();
        Mocha_UnmountFS("storage_mlc");
        Mocha_DeInitLibrary();
        WHBLogConsoleFree();
        isDeinitialized = true;
    }
}


void initialize() {
    OSScreenInit();
    WHBProcInit();

    // Set up the log console for use.
    WHBLogConsoleInit();
    WHBLogConsoleSetColor(0x00009900);

    // Initialize the Mocha library.
    if (Mocha_InitLibrary() != MOCHA_RESULT_SUCCESS) {
        WHBLogConsoleSetColor(0x99000000);
        WHBLogPrint("Mocha_InitLibrary failed!");
        WHBLogConsoleDraw();
        OSSleepTicks(OSMillisecondsToTicks(5000));
        deinitialize();
    }

    // Mount the storage device differently depending on Tiramisu or Aroma.
    Mocha_MountFS("storage_mlc", NULL, "/vol/storage_mlc01");

    // Grab the user's Mii name and persistent ID.
    nn::act::Initialize();
    int16_t miiName[256];
    nn::act::GetMiiName(miiName);
    MII_NICKNAME = std::string(miiName, miiName + sizeof(miiName) / sizeof(miiName[0]));
    USER_ID = nn::act::GetPersistentId();

    // Set the account file path.
    char user_id_hex[9];
    sprintf(user_id_hex, "%08x", USER_ID);
    ACCOUNT_FILE = "storage_mlc:/usr/save/system/act/" + std::string(user_id_hex) + "/account.dat";

    // Set the backup file paths.
    NNID_BACKUP = "fs:/vol/external01/wiiu/accounts/" + std::string(user_id_hex) + "/nnid_account.dat";
    PNID_BACKUP = "fs:/vol/external01/wiiu/accounts/" + std::string(user_id_hex) + "/pnid_account.dat";

    // Set the Inkay configuration file path.
    char environmentPathBuffer[0x100];
    Mocha_GetEnvironmentPath(environmentPathBuffer, sizeof(environmentPathBuffer));
    INKAY_CONFIG = std::string(environmentPathBuffer) + std::string("/plugins/config/inkay.json");
}