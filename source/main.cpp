#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <coreinit/launch.h>
#include <coreinit/screen.h>
#include <coreinit/time.h>
#include <coreinit/thread.h>
#include <mocha/mocha.h>
#include <sysapp/launch.h>
#include <sysapp/title.h>
#include <vpad/input.h>
#include <whb/log_console.h>
#include <whb/log.h>
#include <whb/proc.h>

#define BUFFER_SIZE 0x2000

const char* accountFile = "storage_mlc:/usr/save/system/act/8000000e/account.dat";
const char* inkayConfig = "/vol/external01/wiiu/environments/aroma/plugins/config/inkay.json";

void deinitialize()
{
    Mocha_UnmountFS("storage_mlc");
    Mocha_DeInitLibrary();
    WHBLogConsoleFree();
}

void initialize()
{
    OSScreenInit();
    WHBProcInit();
    // Set up the log console for use.
    WHBLogConsoleInit();
    WHBLogConsoleSetColor(0x00009900);
    // Initialize the Mocha library.
    if (Mocha_InitLibrary() != MOCHA_RESULT_SUCCESS) {
        WHBLogPrint("Mocha_InitLibrary failed!");
        WHBLogConsoleDraw();
        OSSleepTicks(OSMillisecondsToTicks(3000));
        deinitialize();
    }
    // Mount the storage device differently depending on Tiramisu or Aroma.
    Mocha_MountFS("storage_mlc", NULL, "/vol/storage_mlc01");
}


void printOnScreen(int line, const char* format, ...)
{
    char buffer[256];
    va_list args;
    // Format the string into a buffer.
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    // Print the string to the screen.
    OSScreenPutFontEx(SCREEN_TV, 0, line, buffer);
    OSScreenPutFontEx(SCREEN_DRC, 0, line, buffer);
}

void printMainMenu()
{
    OSScreenClearBufferEx(SCREEN_TV, 0x4A198500);
    OSScreenClearBufferEx(SCREEN_DRC, 0x4A198500);

    printOnScreen(0, "Wii U Account Swap (v1.0.0)");
    printOnScreen(1, "Created by Nightkingale");
    printOnScreen(2, "-----------------------------------------------------------");

    printOnScreen(3, "Press A to switch to Nintendo Network ID.");
    printOnScreen(4, "Press B to switch to Pretendo Network ID.");
    printOnScreen(5, "Press X to unlink your account locally.");
    printOnScreen(6, "Press HOME to exit.");

    OSScreenFlipBuffersEx(SCREEN_TV);
    OSScreenFlipBuffersEx(SCREEN_DRC);
}

void printWarningScreen() {
    OSScreenClearBufferEx(SCREEN_TV, 0x4A198500);
    OSScreenClearBufferEx(SCREEN_DRC, 0x4A198500);

    printOnScreen(0, "Warning: This will unlink your account locally!");
    printOnScreen(1, "This will not unlink your account from the server!");
    printOnScreen(2, "-----------------------------------------------------------");
    printOnScreen(3, "Your user can have a new account reattached!");
    printOnScreen(4, "You will be able to login again on this Wii U as normal.");
    printOnScreen(5, "However, you will NOT be able to login to another Wii U!");
    printOnScreen(6, "-----------------------------------------------------------");
    printOnScreen(7, "Press A to confirm the unlink or B to cancel.");

    OSScreenFlipBuffersEx(SCREEN_TV);
    OSScreenFlipBuffersEx(SCREEN_DRC);
}

void unlinkAccount() {
    std::map<std::string, std::string> defaultValues = {
        {"IsMiiUpdated", "1"},
        {"AccountId", ""},
        {"BirthYear", "0"},
        {"BirthMonth", "0"},
        {"BirthDay", "0"},
        {"Gender", "0"},
        {"IsMailAddressValidated", "0"},
        {"EmailAddress", ""},
        {"Country", "0"},
        {"SimpleAddressId", "0"},
        {"TimeZoneId", ""},
        {"UtcOffset", "0"},
        {"PrincipalId", "0"},
        {"NfsPassword", ""},
        {"EciVirtualAccount", ""},
        {"NeedsToDownloadMiiImage", "0"},
        {"MiiImageUrl", ""},
        {"AccountPasswordHash", "0000000000000000000000000000000000000000000000000000000000000000"},
        {"IsPasswordCacheEnabled", "0"},
        {"AccountPasswordCache", "0000000000000000000000000000000000000000000000000000000000000000"},
        {"NnasType", "0"},
        {"NfsType", "0"},
        {"NfsNo", "1"},
        {"NnasSubDomain", ""},
        {"NnasNfsEnv", "L1"},
        {"IsPersistentIdUploaded", "0"},
        {"IsConsoleAccountInfoUploaded", "0"},
        {"LastAuthenticationResult", ""},
        {"StickyAccountId", ""},
        {"NextAccountId", ""},
        {"StickyPrincipalId", "0"},
        {"IsServerAccountDeleted", "0"},
        {"ServerAccountStatus", "1"},
        {"MiiImageLastModifiedDate", "Sat, 01 Jan 2000 00:00:00 GMT"},
        {"IsCommitted", "1"}
    };

    WHBLogPrintf("Unlinking: Default settings will be applied.");
    WHBLogPrintf("%s", accountFile);
    WHBLogPrint("----------------------------------------");
    WHBLogConsoleDraw();

    // Read the entire file into a string.
    std::ifstream inFile(accountFile);
    std::string fileContents((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();
    WHBLogPrint("System account.dat file is in memory!");
    WHBLogConsoleDraw();

    // Process each line in the string.
    std::istringstream iss(fileContents);
    std::string line;
    while (std::getline(iss, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            if (defaultValues.count(key) > 0) {
                line = key + "=" + defaultValues[key];
            }
        }
        fileContents += line + "\n";
    }
    WHBLogPrint("Account file in memory was patched!");
    WHBLogConsoleDraw();

    // Write the string back to the file.
    std::ofstream outFile(accountFile);
    outFile << fileContents;
    outFile.close();
    WHBLogPrint("System account.dat file was saved!");
    WHBLogConsoleDraw();

    WHBLogConsoleSetColor(0x00990000);
    WHBLogPrint("----------------------------------------");
    WHBLogPrint("The account.dat was unlinked successfully!");
    WHBLogPrint("Your console will restart in 3 seconds...");
    WHBLogConsoleDraw();
    
    OSSleepTicks(OSMillisecondsToTicks(3000));
    OSForceFullRelaunch();
    SYSLaunchMenu();
    deinitialize();
}

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

int main()
{
    initialize();
    // Set some variables for the Wii U GamePad.
    VPADStatus input;
    VPADReadError error;

    printMainMenu();

    while (WHBProcIsRunning())
    {
        // Watch the Wii U GamePad for button presses.
        VPADRead(VPAD_CHAN_0, &input, 1, &error);
        // If the A button is pressed, switch to the Nintendo Network ID account.dat.
        if (input.trigger & VPAD_BUTTON_A)
        {
            switchAccount("/vol/external01/accounts/nnid_account.dat", "Nintendo Network ID");
        }
        // If the B button is pressed, switch to the Pretendo Network ID account.dat.
        else if (input.trigger & VPAD_BUTTON_B)
        {
            switchAccount("/vol/external01/accounts/pnid_account.dat", "Pretendo Network ID");
        }
        // If the X button is pressed, unlink the account locally.
        else if (input.trigger & VPAD_BUTTON_X)
        {
            unlinkAccount();
        }
    }

    deinitialize();
    WHBProcShutdown();
    return 0;
}