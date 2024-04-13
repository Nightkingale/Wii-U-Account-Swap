#include <sstream>

#include <coreinit/screen.h>
#include <coreinit/time.h>
#include <coreinit/thread.h>
#include <mocha/mocha.h>
#include <nn/act.h>
#include <vpad/input.h>
#include <whb/log_console.h>
#include <whb/log.h>
#include <whb/proc.h>

#include "../include/global.h"
#include "../include/screen.h"
#include "../include/switch.h"
#include "../include/unlink.h"


const int BUFFER_SIZE = 0x2000;
unsigned int USER_ID;
std::string NNID_BACKUP;
std::string PNID_BACKUP;
std::string MII_NICKNAME;
std::string ACCOUNT_FILE;
std::string INKAY_CONFIG;

void deinitialize() {
    // This prevents hangs when called twice.
    static bool isDeinitialized = false;

    if (!isDeinitialized) {
        nn::act::Finalize();
        Mocha_UnmountFS("storage_mlc");
        Mocha_DeInitLibrary();
        WHBLogConsoleFree();
        WHBProcShutdown();
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

int main() {
    initialize();
    // Initialize variables for the Wii U GamePad.
    VPADStatus input;
    VPADReadError error;
    // Print the main menu to the screen.
    printMainMenu();
    while (WHBProcIsRunning()) {
        printMainMenu();
        // Watch the Wii U GamePad for button presses.
        VPADRead(VPAD_CHAN_0, &input, 1, &error);
        // If the A button is pressed, switch to the Nintendo Network ID account.dat.
        if (input.trigger & VPAD_BUTTON_A) {
            switchAccount(NNID_BACKUP.c_str(), "Nintendo Network ID");
        }
        // If the B button is pressed, switch to the Pretendo Network ID account.dat.
        else if (input.trigger & VPAD_BUTTON_B) {
            switchAccount(PNID_BACKUP.c_str(), "Pretendo Network ID");
        }
        // If the + button is pressed, backup the current account.dat.
        else if (input.trigger & VPAD_BUTTON_PLUS) {
            while (WHBProcIsRunning()) {
                printBackupMenu();
                VPADRead(VPAD_CHAN_0, &input, 1, &error);
                if (input.trigger & VPAD_BUTTON_A) {
                    backupAccount();
                    break;
                }
                else if (input.trigger & VPAD_BUTTON_B) {
                    printMainMenu();
                    break;
                }
            }
        }
        // If the X button is pressed, unlink the account locally.
        else if (input.trigger & VPAD_BUTTON_MINUS) {
            while (WHBProcIsRunning()) {
                printUnlinkMenu();
                VPADRead(VPAD_CHAN_0, &input, 1, &error);
                if (input.trigger & VPAD_BUTTON_A) {
                    unlinkAccount();
                    break;
                }
                else if (input.trigger & VPAD_BUTTON_B) {
                    printMainMenu();
                    break;
                }
            }
        }
    }
    // Deinitialize the program and exit.
    deinitialize();
    return 0;
}