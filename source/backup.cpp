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

#include "../include/global.h"


void writeBackup(FILE* account, const std::string& backupPath, char* buffer) {
    FILE *backup = fopen(backupPath.c_str(), "wb");
    if (backup == NULL) {
        WHBLogConsoleSetColor(0x99000000);
        WHBLogPrintf("Error opening backup file.", backupPath.c_str());
        WHBLogConsoleDraw();
        return;
    }

    WHBLogPrintf("%s", backupPath.c_str());
    WHBLogConsoleDraw();

    // Open the backup file and write the account data to it.
    size_t bytesRead = 0;
    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, account)) > 0) {
        size_t written = fwrite(buffer, 1, bytesRead, backup);
        if (written != bytesRead) {
            WHBLogConsoleSetColor(0x99000000);
            WHBLogPrintf("Error writing to backup file.", backupPath.c_str());
            WHBLogConsoleDraw();
            return;
        }
    }

    fclose(backup);
    WHBLogPrintf("Backup account.dat written.", backupPath);
    WHBLogConsoleDraw();
    // Wait 5 seconds, then go back to the menu.
    WHBLogConsoleSetColor(0x00990000);
    WHBLogPrint("---------------------------------------------------------");
    WHBLogPrint("The account.dat was backed up successfully!");
    WHBLogPrint("The main menu will apppear in 5 seconds...");
    WHBLogConsoleDraw();
    fclose(backup);
}

void backupAccount() {
    WHBLogConsoleSetColor(0x00009900);
    WHBLogPrintf("Backup: A Network ID backup will be created.");
    WHBLogPrint("---------------------------------------------------------");
    WHBLogConsoleDraw();
    // Check if the account.dat file exists.
    FILE *account = fopen(ACCOUNT_FILE.c_str(), "rb");
    if (account == NULL) {
        WHBLogConsoleSetColor(0x99000000);
        WHBLogPrintf("Error opening system account.dat file!");
        WHBLogConsoleDraw();
        // Wait 5 seconds, then go back to the menu.
        OSSleepTicks(OSMillisecondsToTicks(5000));
    }
    else {
        WHBLogPrintf("System account.dat file opened.");
        WHBLogConsoleDraw();
        
        std::string content;
        char *buffer = (char *)malloc(BUFFER_SIZE);
        if (buffer == NULL) {
            WHBLogConsoleSetColor(0x99000000);
            WHBLogPrint("Error allocating memory!");
            WHBLogConsoleDraw();
        }
        else {
            WHBLogPrint("Memory was allocated successfully.");
            WHBLogConsoleDraw();
            // Read the entire file into a string.
            size_t bytesRead = 0;
            while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, account)) > 0) {
                content.append(buffer, bytesRead);
            std::string backupPath;
            WHBLogPrint("account.dat file read in memory.");
            WHBLogConsoleDraw();
            if (content.find("nintendo") == std::string::npos) {
                backupPath = NNID_BACKUP;
                WHBLogPrint("Nintendo Network ID detected.");
                WHBLogConsoleDraw();
            }
            else {
                backupPath = PNID_BACKUP;
                WHBLogPrint("Pretendo Network ID detected.");
                WHBLogConsoleDraw();
            }
            // Check if the backup file exists.
            WHBLogPrintf("Opening backup account.dat for writing.", backupPath.c_str());
            WHBLogConsoleDraw();
            std::ifstream ifile(backupPath);
            if (ifile) {
                printOverwriteMenu(backupPath.c_str());

                VPADStatus input;
                VPADReadError error;

                while (WHBProcIsRunning()) {
                    VPADRead(VPAD_CHAN_0, &input, 1, &error);
                    if (input.trigger == VPAD_BUTTON_A) {
                        writeBackup(account, backupPath, buffer);
                        break;
                    }
                    else if (input.trigger == VPAD_BUTTON_B) {
                        break;
                    }
                }
            }
            else {
                writeBackup(account, backupPath, buffer);
            }
        }
        fclose(account);
        free(buffer);
        OSSleepTicks(OSMillisecondsToTicks(5000));
        printMainMenu();
        }
    }
}