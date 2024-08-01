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

#include "swap.hpp"

#include "main.hpp"
#include "start_screen.hpp"
#include "sub_screens.hpp"


void
handle_cleanup(FILE* backup, account account_type, char* buffer, bool is_error = false)
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
        if (account_type == account::nintendo_network_id)
            draw_start_screen(0);
        else if (account_type == account::pretendo_network_id)
            draw_start_screen(1);
    }
}

bool
user_check(FILE* backup, account account_type)
{
    char *buffer = (char *)malloc(BUFFER_SIZE);
    size_t bytes_read = 0;
    if (buffer == NULL) {
        draw_error_menu("Error allocating memory!");
        handle_cleanup(backup, account_type, buffer, true);
        return false;
    }

    // Prepare the search string, including the user's persistent ID.
    std::string search_string = "PersistentId=" + PERSISTENT_ID;
    bool found = false;
    rewind(backup);

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, backup)) > 0) {
        if (ferror(backup)) {
            draw_error_menu("Error reading from backup account.dat file!");
            handle_cleanup(backup, account_type, buffer, true);
            return false;
        }

        if (strstr(buffer, search_string.c_str()) != NULL) {
            found = true; // The user's persistent ID was found.
            break;
        }
    }

    if (!found) {
        // The persistent ID probably does not match the user's, so block the swap.
        // This means that the end-user probably tried moving backups around. BRICK BLOCKED!
        draw_error_menu("The account.dat does not match the current user!");
        handle_cleanup(backup, account_type, buffer, true);
    }
    return found;
}

bool
swap_account(const char* backup_file, account account_type)
{
    // Disable the HOME Button temporarily.
    OSEnableHomeButtonMenu(0);

    // Open the account.dat file and swap it to the specified account.
    FILE *backup = fopen(backup_file, "rb");
    if (backup == NULL) {
        draw_error_menu("Have you made a backup for this user and account?");
        handle_cleanup(backup, account_type, NULL, true);
        return false;
    }

    // This is a fix to prevent users from using account.dat files that aren't made for it.
    // Moving around backups was an oversight people abused, which led to broken users and bricks.
    if (!user_check(backup, account_type)) {
        return false;
    }

    // Open the account.dat file for writing.
    rewind(backup); // Maybe not necessary but just in case?
    char *buffer = (char *)malloc(BUFFER_SIZE);
    if (buffer == NULL) {
        draw_error_menu("Error allocating memory!");
        handle_cleanup(backup, account_type, buffer, true);
        return false;
    }

    FILE *account = fopen(ACCOUNT_FILE.c_str(), "wb");
    if (account == NULL) {
        draw_error_menu("Error opening system account.dat file!");
        handle_cleanup(backup, account_type, buffer, true);
        return false;
    }

    size_t bytes_read = 0;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, backup)) > 0) {
        if (ferror(backup)) {
            draw_error_menu("Error reading from backup account.dat file!");
            handle_cleanup(backup, account_type, buffer, true);
            return false;
        }

        fwrite(buffer, 1, bytes_read, account);
        if (ferror(account)) {
            draw_error_menu("Error writing to system account.dat file!");
            handle_cleanup(backup, account_type, buffer, true);
            return false;
        }
    }
    fclose(account);

    // We'll attempt to automatically swap the network using Inkay's configuration.
    bool inkay_configured = false;
    FILE *inkay = fopen(INKAY_CONFIG.c_str(), "wb");
    if (inkay != NULL) {
        // Write the network configuration to the file.
        const char *inkay_content = "{\"storageitems\":{\"connect_to_network\":%d}}";
        fprintf(inkay, inkay_content, account_type == account::pretendo_network_id ? 1 : 0);
        fclose(inkay);
        inkay = NULL;
        inkay_configured = true;
    }
    draw_success_menu(success::swap, inkay_configured);

    // Clean-up and exit.
    handle_cleanup(backup, account_type, buffer, false);

    return true;
}