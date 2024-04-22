#include <filesystem>
#include <fstream>
#include <iostream>

#include <string.h>

#include <coreinit/launch.h>
#include <coreinit/screen.h>
#include <coreinit/time.h>
#include <coreinit/thread.h>
#include <mocha/mocha.h>
#include <sysapp/launch.h>
#include <vpad/input.h>
#include <whb/log_console.h>
#include <whb/log.h>
#include <whb/proc.h>

#include "../include/backup.hpp"
#include "../include/main.hpp"
#include "../include/screen.hpp"


bool backupConfirm = false;

void handleCleanup(FILE* account, FILE* backup, char* buffer, bool isError = false) {
    // Wait 5 seconds.
    OSSleepTicks(OSMillisecondsToTicks(5000));

    // Re-enable the HOME Button.
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

    // If there was an error, print the main menu.
    if (isError) {
        WHBLogPrint("---------------------------------------------------------");
        printMainMenu();
    }
}

void writeBackup(FILE* account, const std::string& backupPath, char* buffer) {
    // Create the directories if they don't exist.
    std::filesystem::path dirPath = std::filesystem::path(backupPath).remove_filename();
    std::filesystem::create_directories(dirPath);
    
    // Open the backup file for writing.
    FILE *backup = fopen(backupPath.c_str(), "wb");
    if (backup == NULL) {
        WHBLogConsoleSetColor(0x99000000);
        WHBLogPrintf("Error opening backup file.");
        WHBLogPrintf("%s", backupPath.c_str());
        WHBLogConsoleDraw();
        handleCleanup(account, backup, buffer, true);
        return;
    }

    // Print the backup path to the screen.
    WHBLogPrintf("%s", backupPath.c_str());
    WHBLogConsoleDraw();

    // Open the backup file and write the account data to it.
    rewind(account); // Move the file pointer to the beginning.

    size_t bytesRead = 0;
    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, account)) > 0) {
        fwrite(buffer, 1, bytesRead, backup);
    }

    // Close the backup file.
    fclose(backup);
    WHBLogPrintf("Backup account.dat written.", backupPath);
    WHBLogConsoleDraw();

    // Wait 5 seconds, then go back to the menu.
    WHBLogConsoleSetColor(0x00990000);
    WHBLogPrint("---------------------------------------------------------");
    WHBLogPrint("The account.dat was backed up successfully!");
    WHBLogPrint("The main menu will appear in 5 seconds...");
    WHBLogConsoleDraw();
    WHBLogPrint("---------------------------------------------------------");
    fclose(backup);
}

void backupAccount() {
    // Inform the user that the backup process has started.
    WHBLogConsoleSetColor(0x00009900);
    WHBLogPrintf("Backup: A Network ID backup will be created.");
    WHBLogPrint("---------------------------------------------------------");
    WHBLogConsoleDraw();

    // Check if the account.dat file exists.
    std::string backupPath;
    FILE *account = fopen(ACCOUNT_FILE.c_str(), "rb");
    if (account == NULL) {
        WHBLogConsoleSetColor(0x99000000);
        WHBLogPrintf("Error opening system account.dat file!");
        WHBLogConsoleDraw();
        handleCleanup(account, NULL, NULL, true);
        return;

    } else {
        WHBLogPrintf("System account.dat file opened.");
        WHBLogConsoleDraw();
        
        std::string content;
        char *buffer = (char *)malloc(BUFFER_SIZE);
        if (buffer == NULL) {
            WHBLogConsoleSetColor(0x99000000);
            WHBLogPrint("Error allocating memory!");
            WHBLogConsoleDraw();
            handleCleanup(account, NULL, buffer, true);
            return;

        } else {
            WHBLogPrint("Memory was allocated successfully.");
            WHBLogConsoleDraw();
            // Read the entire file into a string.
            size_t bytesRead = 0;
            while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, account)) > 0) {
                content.append(buffer, bytesRead);
            }

            WHBLogPrint("account.dat file read in memory.");
            WHBLogConsoleDraw();

            bool networkAccountFound = false;
            if (content.find("account.nintendo.net") != std::string::npos) {
                backupPath = NNID_BACKUP;
                WHBLogPrint("Nintendo Network ID detected.");
                WHBLogConsoleDraw();
                networkAccountFound = true;

            } else if (content.find("pretendo-cdn.b-cdn.net") != std::string::npos) {
                backupPath = PNID_BACKUP;
                WHBLogPrint("Pretendo Network ID detected.");
                WHBLogConsoleDraw();
                networkAccountFound = true;

            } else {
                WHBLogConsoleSetColor(0x99000000);
                WHBLogPrint("Network ID detection failed!");
                WHBLogPrint("Is this user a local-only account?");
                WHBLogConsoleDraw();
                handleCleanup(account, NULL, buffer, true);
            }

            if (networkAccountFound) {
                // Check if the backup file exists.
                WHBLogPrintf("Opening backup account.dat for writing.", backupPath.c_str());
                WHBLogConsoleDraw();

                std::ifstream ifile(backupPath);
                if (ifile) {
                    VPADStatus input;
                    VPADReadError error;

                    backupConfirm = false;

                    while (WHBProcIsRunning()) {
                        printOverwriteMenu(backupPath.c_str());
                        VPADRead(VPAD_CHAN_0, &input, 1, &error);

                        if (input.trigger == VPAD_BUTTON_A) {
                            backupConfirm = true;
                            break;
                        } else if (input.trigger == VPAD_BUTTON_B) {
                            break;
                        }
                    }

                } else {
                    backupConfirm = true;
                }
            }
        }
        // Write the backup file.
        if (backupConfirm) {
            writeBackup(account, backupPath, buffer);
        }

        // Handle cleanup
        handleCleanup(account, NULL, buffer, !backupConfirm);
        }
}