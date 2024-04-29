#include <sstream>

#include <coreinit/screen.h>
#include <coreinit/time.h>
#include <coreinit/thread.h>
#include <vpad/input.h>
#include <whb/proc.h>

#include "backup.hpp"
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

    // Initialize variables for the Wii U GamePad.
    VPADStatus input;
    VPADReadError error;

    // Print the main menu to the screen.
    print_main_menu();

    while (WHBProcIsRunning()) {
        print_main_menu();

        // Watch the Wii U GamePad for button presses.
        VPADRead(VPAD_CHAN_0, &input, 1, &error);

        // If the A button is pressed, switch to the Nintendo Network ID account.dat.
        if (input.trigger & VPAD_BUTTON_A) {
            switch_account(NNID_BACKUP.c_str(), "Nintendo Network ID");
        }

        // If the B button is pressed, switch to the Pretendo Network ID account.dat.
        else if (input.trigger & VPAD_BUTTON_B) {
            switch_account(PNID_BACKUP.c_str(), "Pretendo Network ID");
        }

        // If the + button is pressed, backup the current account.dat.
        else if (input.trigger & VPAD_BUTTON_PLUS) {
            while (WHBProcIsRunning()) {
                // Disable the HOME Button temporarily.
                OSEnableHomeButtonMenu(0);
                print_backup_menu();

                VPADRead(VPAD_CHAN_0, &input, 1, &error);

                if (input.trigger & VPAD_BUTTON_A) {
                    backup_account();
                    break;
                }

                else if (input.trigger & VPAD_BUTTON_B) {
                    // Re-enable the HOME Button.
                    OSEnableHomeButtonMenu(1);
                    print_main_menu();
                    break;
                }
            }
        }

        // If the X button is pressed, unlink the account locally.
        else if (input.trigger & VPAD_BUTTON_MINUS) {
            while (WHBProcIsRunning()) {
                // Disable the HOME Button temporarily.
                OSEnableHomeButtonMenu(0);
                print_unlink_menu();
                
                VPADRead(VPAD_CHAN_0, &input, 1, &error);
                
                if (input.trigger & VPAD_BUTTON_A) {
                    unlink_account();
                    break;
                }

                else if (input.trigger & VPAD_BUTTON_B) {
                    // Re-enable the HOME Button.
                    OSEnableHomeButtonMenu(1);
                    print_main_menu();
                    break;
                }
            }
        }
    }
    // Deinitialize the program and exit.
    deinitialize();
    return 0;
}