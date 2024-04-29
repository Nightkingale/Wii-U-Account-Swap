#include <cstring>
#include <sstream>

#include <coreinit/screen.h>
#include <coreinit/time.h>
#include <coreinit/thread.h>
#include <padscore/kpad.h>
#include <vpad/input.h>
#include <whb/proc.h>

#include "backup.hpp"
#include "input.hpp"
#include "main.hpp"
#include "screen.hpp"
#include "switch.hpp"
#include "unlink.hpp"
#include "utils.hpp"

const int BUFFER_SIZE = 0x2000; // The buffer size allocated for reading and writing files.

unsigned int USER_ID; // The current user persistant ID.
std::string NNID_BACKUP; // The backup path to the Nintendo Network ID account.dat.
std::string PNID_BACKUP; // The backup path to the Pretendo Network ID account.dat.
std::string MII_NICKNAME; // The current user's Mii nickname.
std::string ACCOUNT_FILE; // The path to the current account.dat.
std::string INKAY_CONFIG; // The path to the Inkay configuration file.


int main() {
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