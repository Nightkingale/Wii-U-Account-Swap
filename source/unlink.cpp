#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <stdlib.h>
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

#include "../include/main.hpp"
#include "../include/utils.hpp"


void unlink_account() {
    // The default values to apply to the account.dat file.
    std::map<std::string, std::string> default_values = {
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
    
    // Inform the user that the unlink process has started.
    WHBLogConsoleSetColor(0x00009900);
    WHBLogPrintf("Unlinking: Default settings will be applied.");
    WHBLogPrint("---------------------------------------------------------");
    WHBLogConsoleDraw();

    // Read the entire file into a string.
    std::ifstream account_input(ACCOUNT_FILE);
    std::string file_contents((std::istreambuf_iterator<char>(account_input)), std::istreambuf_iterator<char>());
    account_input.close();
    WHBLogPrint("System account.dat file read in memory.");
    WHBLogConsoleDraw();

    // Process each line in the string.
    std::istringstream file_contentstream(file_contents);
    std::string line;
    while (std::getline(file_contentstream, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            if (default_values.count(key) > 0) {
                line = key + "=" + default_values[key];
            }
        }
        file_contents += line + "\n";
    }

    WHBLogPrint("Account file in memory patched.");
    WHBLogConsoleDraw();

    // Write the string back to the file.
    std::ofstream account_output(ACCOUNT_FILE);
    account_output << file_contents;
    account_output.close();
    WHBLogPrint("System account.dat file written.");
    WHBLogConsoleDraw();

    // Inform the user that the unlink was successful.
    WHBLogConsoleSetColor(0x00990000);
    WHBLogPrint("---------------------------------------------------------");
    WHBLogPrint("The account.dat was unlinked successfully!");
    WHBLogPrint("Your console will restart in 5 seconds...");
    WHBLogConsoleDraw();
    WHBLogPrint("---------------------------------------------------------");

    // Wait 5 seconds, then soft reboot the console.
    OSSleepTicks(OSMillisecondsToTicks(5000));

    // Re-enable the HOME Button.
    OSEnableHomeButtonMenu(1);
    deinitialize();
    OSForceFullRelaunch();
    SYSLaunchMenu();
}
