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


const char* NNID_BACKUP = "/vol/external01/accounts/nnid_account.dat";
const char* PNID_BACKUP = "/vol/external01/accounts/pnid_account.dat";
const char* INKAY_CONFIG = "/vol/external01/wiiu/environments/aroma/plugins/config/inkay.json";
const int BUFFER_SIZE = 0x2000;

unsigned int USER_ID;
std::string MII_NICKNAME;
std::string ACCOUNT_FILE;

void deinitialize() {
    nn::act::Finalize();
    Mocha_UnmountFS("storage_mlc");
    Mocha_DeInitLibrary();
    WHBLogConsoleFree();
}

void initialize() {
    OSScreenInit();
    WHBProcInit();
    // Set up the log console for use.
    WHBLogConsoleInit();
    WHBLogConsoleSetColor(0x00009900);
    // Initialize the Mocha library.
    if (Mocha_InitLibrary() != MOCHA_RESULT_SUCCESS) {
        WHBLogPrint("Mocha_InitLibrary failed!");
        WHBLogConsoleDraw();
        OSSleepTicks(OSMillisecondsToTicks(3000));
        deinitialize();
    }
    // Mount the storage device differently depending on Tiramisu or Aroma.
    Mocha_MountFS("storage_mlc", NULL, "/vol/storage_mlc01");
    
    nn::act::Initialize();
    int16_t miiName[256];
    nn::act::GetMiiName(miiName);
    MII_NICKNAME = std::string(miiName, miiName + sizeof(miiName) / sizeof(miiName[0]));
    USER_ID = nn::act::GetPersistentId();

    char user_id_hex[9];
    sprintf(user_id_hex, "%08x", USER_ID);
    ACCOUNT_FILE = "storage_mlc:/usr/save/system/act/" + std::string(user_id_hex) + "/account.dat";
}

int main() {
    initialize();

    VPADStatus input;
    VPADReadError error;

    printMainMenu();

    while (WHBProcIsRunning()) {
        // Watch the Wii U GamePad for button presses.
        VPADRead(VPAD_CHAN_0, &input, 1, &error);
        // If the A button is pressed, switch to the Nintendo Network ID account.dat.
        if (input.trigger & VPAD_BUTTON_A) {
            switchAccount(NNID_BACKUP, "Nintendo Network ID");
        }
        // If the B button is pressed, switch to the Pretendo Network ID account.dat.
        else if (input.trigger & VPAD_BUTTON_B) {
            switchAccount(PNID_BACKUP, "Pretendo Network ID");
        }
        // If the X button is pressed, unlink the account locally.
        else if (input.trigger & VPAD_BUTTON_X) {
            printWarningMenu();
            while (WHBProcIsRunning()) {
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

    deinitialize();
    WHBProcShutdown();
    return 0;
}