#include <filesystem>
#include <sstream>
#include <string>

#include <coreinit/thread.h>

#include "input.hpp"
#include "main.hpp"
#include "start_screen.hpp"
#include "sub_screens.hpp"


bool backup_confirm = false;


void
handle_cleanup(FILE* account, FILE* backup, char* buffer, bool is_error = false)
{
    OSEnableHomeButtonMenu(1);

    // Free the buffer.
    if (buffer != NULL) {
        free(buffer);
        buffer = NULL;
    }

    // Close the account file.
    if (account != NULL) {
        fclose(account);
        account = NULL;
    }

    // Close the backup file.
    if (backup != NULL) {
        fclose(backup);
        backup = NULL;
    }

    if (is_error) {
        draw_start_screen(2);
    }
}


bool
write_backup(FILE* account, const std::string& backup_path, char* buffer)
{
    // Create the directories if they don't exist.
    std::filesystem::path dir_path = std::filesystem::path(backup_path).remove_filename();
    std::filesystem::create_directories(dir_path);
    
    // Open the backup file for writing.
    FILE *backup = fopen(backup_path.c_str(), "wb");

    if (backup == NULL) {
        draw_error_menu("Error opening backup account.dat file!");
        handle_cleanup(account, backup, buffer, true);
        return false;
    }

    // Open the backup file and write the account data to it.
    rewind(account); // Move the file pointer to the beginning.

    size_t bytes_read = 0;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, account)) > 0)
        fwrite(buffer, 1, bytes_read, backup);

    // Check if there was an error when writing.
    if (ferror(backup)) {
        draw_error_menu("Error writing to backup account.dat file!");
        handle_cleanup(account, backup, buffer, true);
        return false;
    }

    // Close the backup file.
    fclose(backup);

    draw_success_menu(success::backup);
    return true;
}


bool
backup_account()
{
    // Check if the account.dat file exists.
    std::string backup_path;
    FILE *account = fopen(ACCOUNT_FILE.c_str(), "rb");
    if (account == NULL) {
        draw_error_menu("Error opening system account.dat file!");
        handle_cleanup(account, NULL, NULL, true);
        return false;
    }

    std::string content;
    char *buffer = (char *)malloc(BUFFER_SIZE);
    if (buffer == NULL) {
        draw_error_menu("Error allocating memory!");
        handle_cleanup(account, NULL, buffer, true);
        return false;
    }

    // Read the entire file into a string.
    size_t bytes_read = 0;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, account)) > 0) {
        content.append(buffer, bytes_read);
    }

    bool network_account_found = false;
    std::istringstream file_contentstream(content);
    std::string line;

    while (std::getline(file_contentstream, line)) {
        if (line.find("MiiImageUrl") != std::string::npos) {
            if (line.find("nintendo") != std::string::npos) {
                // Nintendo Network ID is linked to the account.
                backup_path = NNID_BACKUP;
                network_account_found = true;
                break;
            } else if (line.find("pretendo") != std::string::npos) {
                // Pretendo Network ID is linked to the account.
                backup_path = PNID_BACKUP;
                network_account_found = true;
                break;
            }
        }
    }

    if (!network_account_found) {
        // The check failed, domain not accounted for?
        draw_error_menu("Do you have a Network ID linked to this user?");
        handle_cleanup(account, NULL, buffer, true);
        return false;
    }

    current_screen = overwrite_screen;

    // Check if the backup file exists.
    if (std::filesystem::exists(backup_path)) {
        backup_confirm = false;

        while (true) {
            draw_overwrite_menu(backup_path.c_str());
            int button = read_input();

            if (button & VPAD_BUTTON_A) {
                backup_confirm = true;
                break;
            } else if (button & VPAD_BUTTON_B) {
                break;
            }
        }
    } else {
        backup_confirm = true;
    }

    current_screen = backup_screen;

    // Write the backup file.
    if (backup_confirm) {
        if (write_backup(account, backup_path, buffer))
        {
            handle_cleanup(account, NULL, buffer, false);
            current_screen = start_screen;
        }
        return true;
    }

    handle_cleanup(account, NULL, buffer, !backup_confirm);
    current_screen = start_screen;
    return true;
}