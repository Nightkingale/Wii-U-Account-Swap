#include <sstream>

#include <coreinit/screen.h>
#include <coreinit/time.h>
#include <coreinit/thread.h>
#include <vpad/input.h>
#include <whb/proc.h>

#include "../include/backup.hpp"
#include "../include/main.hpp"
#include "../include/screen.hpp"
#include "../include/switch.hpp"
#include "../include/unlink.hpp"
#include "../include/utils.hpp"

const std::string VERSION_NUMBER = "v1.0.0-rc1"; // The current version number of the program.
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
                // Disable the HOME Button temporarily.
                OSEnableHomeButtonMenu(0);
                printBackupMenu();

                VPADRead(VPAD_CHAN_0, &input, 1, &error);

                if (input.trigger & VPAD_BUTTON_A) {
                    backupAccount();
                    break;
                }

                else if (input.trigger & VPAD_BUTTON_B) {
                    // Re-enable the HOME Button.
                    OSEnableHomeButtonMenu(1);
                    printMainMenu();
                    break;
                }
            }
        }

        // If the X button is pressed, unlink the account locally.
        else if (input.trigger & VPAD_BUTTON_MINUS) {
            while (WHBProcIsRunning()) {
                // Disable the HOME Button temporarily.
                OSEnableHomeButtonMenu(0);
                printUnlinkMenu();
                
                VPADRead(VPAD_CHAN_0, &input, 1, &error);
                
                if (input.trigger & VPAD_BUTTON_A) {
                    unlinkAccount();
                    break;
                }

                else if (input.trigger & VPAD_BUTTON_B) {
                    // Re-enable the HOME Button.
                    OSEnableHomeButtonMenu(1);
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