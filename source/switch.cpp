#include <iostream>

#include <string.h>

#include <coreinit/launch.h>
#include <coreinit/screen.h>
#include <coreinit/time.h>
#include <coreinit/thread.h>
#include <mocha/mocha.h>
#include <sysapp/launch.h>
#include <whb/log_console.h>
#include <whb/log.h>
#include <whb/proc.h>

#include "globals.h"


void switchAccount(const char* backupFile, const char* accountType)
{
    WHBLogPrintf("Switching to %s...", accountType);
    WHBLogPrintf("Source: %s", backupFile);
    WHBLogPrintf("%s", accountFile);
    WHBLogPrint("----------------------------------------");
    WHBLogConsoleDraw();

    WHBLogPrintf("Switching account.dat file to %s.", accountType);
    WHBLogConsoleDraw();
    // Open the account.dat file and switch it to the specified account.
    FILE *backup = fopen(backupFile, "rb");
    if (backup == NULL)
    {
        WHBLogConsoleSetColor(0x99000000);
        WHBLogPrintf("Error opening %s account backup!", accountType);
        WHBLogConsoleDraw();
    }
    else
    {
        WHBLogPrintf("%s account backup opened!", accountType);
        WHBLogConsoleDraw();
        char *buffer = (char *)malloc(BUFFER_SIZE);
        if (buffer == NULL)
        {
            WHBLogConsoleSetColor(0x99000000);
            WHBLogPrint("Error allocating memory!");
            WHBLogConsoleDraw();
        }
        else
        {
            WHBLogPrint("Memory was allocated successfully!");
            WHBLogConsoleDraw();
            
            FILE *account = fopen(accountFile, "wb");
            if (account == NULL)
            {
                WHBLogConsoleSetColor(0x99000000);
                WHBLogPrint("Error opening system account.dat file!");
                WHBLogConsoleDraw();
            }
            else
            {
                WHBLogPrint("System account.dat file opened!");
                WHBLogConsoleDraw();
                size_t bytesRead = 0;
                while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, backup)) > 0)
                {
                    fwrite(buffer, 1, bytesRead, account);
                }
                fclose(account);
                WHBLogConsoleSetColor(0x00990000);
                WHBLogPrint("System account.dat file was saved!");
                // We'll attempt to automatically swap the network using Inkay's configuration.
                FILE *inkay = fopen(inkayConfig, "wb");
                if (inkay == NULL)
                {
                    // If we can't open the file, we will move on.
                    WHBLogPrint("Error opening Inkay config file!");
                    WHBLogPrint("Network will not be automatically swapped.");
                    WHBLogConsoleDraw();
                }
                else
                {
                    WHBLogPrint("Inkay config file opened!");
                    WHBLogConsoleDraw();
                    WHBLogPrintf("Swapping network to %s!", accountType);
                    const char *inkayContent = "{\"storageitems\":{\"connect_to_network\":%d}}";
                    fprintf(inkay, inkayContent, strcmp(accountType, "Pretendo Network ID") == 0 ? 1 : 0);
                    fclose(inkay);
                    WHBLogPrint("Inkay config file was saved!");
                    WHBLogConsoleDraw();
                }
                WHBLogPrint("----------------------------------------");
                WHBLogPrint("The account.dat was restored successfully!");
                WHBLogPrint("Your console will restart in 3 seconds...");
                WHBLogConsoleDraw();
            }
            free(buffer);
        }
        fclose(backup);
    }
    OSSleepTicks(OSMillisecondsToTicks(3000));
    OSForceFullRelaunch();
    SYSLaunchMenu();
    deinitialize();
}