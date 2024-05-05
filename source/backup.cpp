#include <filesystem>
#include <fstream>
#include <iostream>

#include <coreinit/launch.h>
#include <coreinit/screen.h>
#include <coreinit/thread.h>
#include <coreinit/time.h>
#include <mocha/mocha.h>
#include <sysapp/launch.h>
#include <vpad/input.h>
#include <whb/log.h>
#include <whb/log_console.h>
#include <whb/proc.h>

#include "input.hpp"
#include "main.hpp"
#include "screen.hpp"


bool backup_confirm = false;


void handle_cleanup(FILE* account, FILE* backup, char* buffer, bool is_error = false) {
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
        draw_menu_screen(2);
    }
}


void write_backup(FILE* account, const std::string& backup_path, char* buffer) {
    // Create the directories if they don't exist.
    std::filesystem::path dirPath = std::filesystem::path(backup_path).remove_filename();
    std::filesystem::create_directories(dirPath);
    
    // Open the backup file for writing.
    FILE *backup = fopen(backup_path.c_str(), "wb");
    if (backup == NULL) {
        draw_error_menu("Error opening backup account.dat file!");
        handle_cleanup(account, backup, buffer, true);
        return;
    }

    // Open the backup file and write the account data to it.
    rewind(account); // Move the file pointer to the beginning.

    size_t bytesRead = 0;
    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, account)) > 0)
        fwrite(buffer, 1, bytesRead, backup);

    // Close the backup file.
    fclose(backup);

    draw_success_menu("backup");
}


bool backup_account() {
    // Check if the account.dat file exists.
    std::string backup_path;
    FILE *account = fopen(ACCOUNT_FILE.c_str(), "rb");
    if (account == NULL) {
        draw_error_menu("Error opening system account.dat file!");
        handle_cleanup(account, NULL, NULL, true);
        return false;

    } else {
        std::string content;
        char *buffer = (char *)malloc(BUFFER_SIZE);
        if (buffer == NULL) {
            draw_error_menu("Error allocating memory!");
            handle_cleanup(account, NULL, buffer, true);
            return false;

        } else {
            // Read the entire file into a string.
            size_t bytesRead = 0;
            while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, account)) > 0) {
                content.append(buffer, bytesRead);
            }

            bool network_account_found = false;
            if (content.find("account.nintendo.net") != std::string::npos) {
                // Nintendo Network ID is linked to the account.
                backup_path = NNID_BACKUP;
                network_account_found = true;

            } else if (content.find("pretendo-cdn.b-cdn.net") != std::string::npos) {
                // Pretendo Network ID is linked to the account.
                backup_path = PNID_BACKUP;
                network_account_found = true;

            } else {
                // The check failed, domain not accounted for?
                draw_error_menu("No network account found!");
                handle_cleanup(account, NULL, buffer, true);
                return false;
            }

            if (network_account_found) {
                // Check if the backup file exists.
                std::ifstream ifile(backup_path);
                if (ifile) {
                    backup_confirm = false;

                    while (WHBProcIsRunning()) {
                        draw_overwrite_menu(backup_path.c_str());
                        int button = read_input();

                        if (button == VPAD_BUTTON_A) {
                            backup_confirm = true;
                            break;
                        } else if (button == VPAD_BUTTON_B) {
                            break;
                        }
                    }

                } else {
                    backup_confirm = true;
                }
            }
        }
        // Write the backup file.
        if (backup_confirm) {
            write_backup(account, backup_path, buffer);
        }

        handle_cleanup(account, NULL, buffer, !backup_confirm);
        return true;
    }
}