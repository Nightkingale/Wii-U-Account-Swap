#include <cstring>
#include <sstream>

#include <coreinit/screen.h>
#include <coreinit/thread.h>
#include <coreinit/time.h>
#include <mocha/mocha.h>
#include <nn/act.h>
#include <padscore/kpad.h>
#include <vpad/input.h>
#include <whb/log.h>
#include <whb/log_console.h>
#include <whb/proc.h>

#include "backup.hpp"
#include "graphics.hpp"
#include "input.hpp"
#include "main.hpp"
#include "screen.hpp"
#include "switch.hpp"
#include "unlink.hpp"


unsigned int USER_ID; // The current user persistant ID.
std::string NNID_BACKUP; // The backup path to the Nintendo Network ID account.dat.
std::string PNID_BACKUP; // The backup path to the Pretendo Network ID account.dat.
std::string MII_NICKNAME; // The current user's Mii nickname.
std::string ACCOUNT_FILE; // The path to the current account.dat.
std::string INKAY_CONFIG; // The path to the Inkay configuration file.


void get_user_information() {
    // Grab the user's Mii name and persistent ID.
    nn::act::Initialize();
    int16_t mii_name[256];
    nn::act::GetMiiName(mii_name);
    MII_NICKNAME = std::string(mii_name, mii_name + sizeof(mii_name) / sizeof(mii_name[0]));
    USER_ID = nn::act::GetPersistentId();

    // Set the account file path.
    char user_id_hex[9];
    sprintf(user_id_hex, "%08x", USER_ID);
    ACCOUNT_FILE = "storage_mlc:/usr/save/system/act/" + std::string(user_id_hex) + "/account.dat";

    // Set the backup file paths.
    NNID_BACKUP = "fs:/vol/external01/wiiu/accounts/" + std::string(user_id_hex) + "/nnid_account.dat";
    PNID_BACKUP = "fs:/vol/external01/wiiu/accounts/" + std::string(user_id_hex) + "/pnid_account.dat";

    // Set the Inkay configuration file path.
    char environment_path_buffer[0x100];
    Mocha_GetEnvironmentPath(environment_path_buffer, sizeof(environment_path_buffer));
    INKAY_CONFIG = std::string(environment_path_buffer) + std::string("/plugins/config/inkay.json");
}


void deinitialize() {
    // This prevents hangs when called twice.
    static bool is_deinitialized = false;

    if (!is_deinitialized) {
        nn::act::Finalize();
        Mocha_UnmountFS("storage_mlc");
        Mocha_DeInitLibrary();
        WHBLogConsoleFree();
        VPADShutdown();
        KPADShutdown();
        is_deinitialized = true;
    }
}


void initialize() {
    OSScreenInit();
    WHBProcInit();
    VPADInit();
    KPADInit();
    WPADEnableURCC(1);

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
    get_user_information();
}


int main() {
    new_graphics(); // Shows the work-in-progress GUI. Press X to continue to the main menu.
    initialize();

    while (WHBProcIsRunning()) {
        print_main_menu();

        int button = read_input(); // Watch the controllers for input.

        if (button == VPAD_BUTTON_A)
            // Press (A) to switch to Nintendo Network ID.
            switch_account(NNID_BACKUP.c_str(), "Nintendo Network ID");

        else if (button == VPAD_BUTTON_B)
            // Press (B) to switch to Pretendo Network ID.
            switch_account(PNID_BACKUP.c_str(), "Pretendo Network ID");

        else if (button == VPAD_BUTTON_PLUS) {
            // Press (+) to backup the current account.
            while (WHBProcIsRunning()) {
                OSEnableHomeButtonMenu(0);
                print_backup_menu();

                button = read_input();

                if (button == VPAD_BUTTON_A) {
                    backup_account();
                    break;
                }

                else if (button == VPAD_BUTTON_B) {
                    OSEnableHomeButtonMenu(1);
                    print_main_menu();
                    break;
                }
            }
        }

        else if (button == VPAD_BUTTON_MINUS) {
            // Press (-) to unlink the current account.
            while (WHBProcIsRunning()) {
                OSEnableHomeButtonMenu(0);
                print_unlink_menu();
                
                button = read_input();
                
                if (button == VPAD_BUTTON_A) {
                    unlink_account();
                    break;
                }

                else if (button == VPAD_BUTTON_B) {
                    OSEnableHomeButtonMenu(1);
                    print_main_menu();
                    break;
                }
            }
        }
    }

    deinitialize();
    return 0;
}