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


void
handle_cleanup(FILE* backup, const char* account_type, char* buffer, bool is_error = false)
{
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
        if (strcmp(account_type, "Nintendo Network ID"))
            draw_menu_screen(0);
        else if (strcmp(account_type, "Pretendo Network ID"))
            draw_menu_screen(1);
    }
}


bool
switch_account(const char* backup_file, const char* account_type)
{
    // Disable the HOME Button temporarily.
    OSEnableHomeButtonMenu(0);

    // Open the account.dat file and switch it to the specified account.
    FILE *backup = fopen(backup_file, "rb");
    if (backup == NULL) {
        draw_error_menu("Error opening backup account.dat file!");
        handle_cleanup(backup, account_type, NULL, true);
        return false;
    }
    else {
        // Open the account.dat file for writing.
        char *buffer = (char *)malloc(BUFFER_SIZE);
        if (buffer == NULL) {
            draw_error_menu("Error allocating memory!");
            handle_cleanup(backup, account_type, buffer, true);
            return false;

        } else {
            FILE *account = fopen(ACCOUNT_FILE.c_str(), "wb");
            if (account == NULL) {
                draw_error_menu("Error opening system account.dat file!");
                handle_cleanup(backup, account_type, buffer, true);
                return false;

            } else {
                size_t bytesRead = 0;
                while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, backup)) > 0)
                    fwrite(buffer, 1, bytesRead, account);
                fclose(account);

                // We'll attempt to automatically swap the network using Inkay's configuration.
                bool inkay_configured = false;
                FILE *inkay = fopen(INKAY_CONFIG.c_str(), "wb");
                if (inkay != NULL) {
                    // Write the network configuration to the file.
                    const char *inkay_content = "{\"storageitems\":{\"connect_to_network\":%d}}";
                    fprintf(inkay, inkay_content, strcmp(account_type, "Pretendo Network ID") == 0 ? 1 : 0);
                    fclose(inkay);
                    inkay = NULL;
                    inkay_configured = true;
                }
                draw_success_menu("switch", inkay_configured);
            }
        }
        // Clean-up and exit.
        handle_cleanup(backup, account_type, buffer, false);

        return true;
    }
}