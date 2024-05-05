#include <cstring>
#include <iostream>

#include <coreinit/launch.h>
#include <coreinit/screen.h>
#include <coreinit/thread.h>
#include <coreinit/time.h>
#include <mocha/mocha.h>
#include <sysapp/launch.h>
#include <whb/log.h>
#include <whb/log_console.h>
#include <whb/proc.h>

#include "main.hpp"
#include "screen.hpp"


void handle_cleanup(FILE* backup, const char* account_type, char* buffer, bool is_error = false) {
    OSSleepTicks(OSMillisecondsToTicks(5000));
    OSEnableHomeButtonMenu(1);
    
    // Free the buffer.
    if (buffer != NULL) {
        free(buffer);
        buffer = NULL;
    }
    
    // Close the backup file.
    if (backup != NULL) {
        fclose(backup);
        backup = NULL;
    }
    
    // If there was an error, return to the menu.
    if (is_error) {
        // Print the main menu.
        WHBLogPrint("---------------------------------------------------------");

        if (strcmp(account_type, "Nintendo Network ID"))
            draw_menu_screen(0);
        else if (strcmp(account_type, "Pretendo Network ID"))
            draw_menu_screen(1);
    }
}


void switch_account(const char* backup_file, const char* account_type) {
    // Disable the HOME Button temporarily.
    OSEnableHomeButtonMenu(0);

    // Print the account type and the switch message.
    WHBLogConsoleSetColor(0x00009900);
    WHBLogPrintf("Switch: You will be swapped to a %s.", account_type);
    WHBLogPrint("---------------------------------------------------------");
    WHBLogConsoleDraw();

    WHBLogPrintf("Switching account.dat to %s.", account_type);
    WHBLogConsoleDraw();

    // Open the account.dat file and switch it to the specified account.
    FILE *backup = fopen(backup_file, "rb");
    if (backup == NULL) {
        WHBLogConsoleSetColor(0x99000000);
        WHBLogPrintf("Error opening %s account backup!", account_type);
        WHBLogPrint("Have you made a backup of this account yet?");
        WHBLogConsoleDraw();
        handle_cleanup(backup, account_type, NULL, true);
        return;
    }
    else {
        // Open the account.dat file for writing.
        WHBLogPrintf("%s account backup opened.", account_type);
        WHBLogConsoleDraw();

        char *buffer = (char *)malloc(BUFFER_SIZE);
        if (buffer == NULL) {
            WHBLogConsoleSetColor(0x99000000);
            WHBLogPrint("Error allocating memory!");
            WHBLogConsoleDraw();
            handle_cleanup(backup, account_type, buffer, true);
            return;

        } else {
            WHBLogPrint("Memory was allocated successfully.");
            WHBLogConsoleDraw();
            
            FILE *account = fopen(ACCOUNT_FILE.c_str(), "wb");
            if (account == NULL) {
                WHBLogConsoleSetColor(0x99000000);
                WHBLogPrint("Error opening system account.dat file!");
                WHBLogConsoleDraw();
                handle_cleanup(backup, account_type, buffer, true);
                return;

            } else {
                WHBLogPrint("System account.dat file opened.");
                WHBLogConsoleDraw();

                size_t bytesRead = 0;
                while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, backup)) > 0)
                    fwrite(buffer, 1, bytesRead, account);
                fclose(account);

                WHBLogPrint("System account.dat file restored.");

                // We'll attempt to automatically swap the network using Inkay's configuration.
                FILE *inkay = fopen(INKAY_CONFIG.c_str(), "wb");
                if (inkay == NULL) {
                    // If we can't open the file, we will move on.
                    WHBLogPrint("The Inkay config file wasn't found!");
                    WHBLogPrint("Network will not be automatically swapped.");
                    WHBLogConsoleDraw();

                } else {
                    // Write the network configuration to the file.
                    WHBLogPrint("Inkay config file opened.");
                    WHBLogConsoleDraw();
                    WHBLogPrintf("Swapping network to %s.", account_type);

                    const char *inkayContent = "{\"storageitems\":{\"connect_to_network\":%d}}";
                    fprintf(inkay, inkayContent, strcmp(account_type, "Pretendo Network ID") == 0 ? 1 : 0);
                    fclose(inkay);
                    inkay = NULL;

                    WHBLogPrint("Inkay config file edited.");
                    WHBLogConsoleDraw();
                }
                // Inform the user that the switch was successful.
                WHBLogConsoleSetColor(0x00990000);
                WHBLogPrint("---------------------------------------------------------");
                WHBLogPrint("The account.dat was restored successfully!");
                WHBLogPrint("Your console will restart in 5 seconds...");
                WHBLogConsoleDraw();
            }
        }
        // Clean-up and exit.
        handle_cleanup(backup, account_type, buffer, false);

        OSForceFullRelaunch();
        SYSLaunchMenu();
        deinitialize();
    }
}