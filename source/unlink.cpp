#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <vector>

#include <coreinit/launch.h>
#include <coreinit/screen.h>
#include <coreinit/time.h>
#include <coreinit/thread.h>
#include <mocha/mocha.h>
#include <sysapp/launch.h>
#include <whb/log.h>
#include <whb/log_console.h>
#include <whb/proc.h>

#include "main.hpp"
#include "sub_screens.hpp"


bool
unlink_account()
{
    // The values to leave in the account.dat file.
    std::vector<std::string> retain_values = {
        "PersistentId",
        "TransferableIdBase",
        "Uuid",
        "ParentalControlSlotNo",
        "MiiData",
        "MiiName"
    };

    // The default values to apply to the account.dat file.
    std::map<std::string, std::string> reset_values = {
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

    // Read the entire file into a string.
    std::ifstream account_input(ACCOUNT_FILE);

    if (!account_input.is_open()) {
        draw_error_menu("Error opening system account.dat file!");
        OSEnableHomeButtonMenu(1);
        return false;
    }

    std::string file_contents((std::istreambuf_iterator<char>(account_input)), std::istreambuf_iterator<char>());
    account_input.close();

    // We need to process the file line by line.
    std::istringstream file_contentstream(file_contents);
    std::string line;
    std::string processed_contents; // New string to hold the processed content.
    std::unordered_set<std::string> encountered_lines; // Set to store encountered lines.
    while (std::getline(file_contentstream, line)) {
        if (encountered_lines.count(line) > 0) { // Skip duplicate lines.
            continue;
        }

        size_t pos = line.find('=');
        if (pos == std::string::npos) {
            if (line == "AccountInstance_20120705") {
                // This line is a special case, it is best to retain it at the top.
                processed_contents += line + "\n"; // Append to processed_contents.
                encountered_lines.insert(line) ; // Insert into encountered_lines.
            }
            continue;
        }

        // We will only keep other lines if they are in one of the sets, to either retain or reset.
        // This is to fix when the account.dat file appended rather than replaced in the v1.0.0 codebase.
        // The system would clean up the file but leave some traces. Hopefully this will fix that!
        std::string key = line.substr(0, pos);
        if (std::find(retain_values.begin(), retain_values.end(), key) != retain_values.end()) {
            // It's a value we want to retain (basic Mii information and data).
            processed_contents += line + "\n";
            encountered_lines.insert(line);
        } else if (reset_values.count(key) > 0) {
            // It's a value we want to reset (anything related to Network ID).
            line = key + "=" + reset_values[key];
            processed_contents += line + "\n";
            encountered_lines.insert(line);
        }
    }

    // Write the string back to the file.
    std::ofstream account_output(ACCOUNT_FILE);
    account_output << processed_contents; // Write processed_contents to the file.

    if (account_output.bad()) {
        draw_error_menu("Error writing to system account.dat file!");
        OSEnableHomeButtonMenu(1);
        return false;
    }

    account_output.close();

    draw_success_menu(success::unlink); // Draw the success menu.
    OSEnableHomeButtonMenu(1);

    return true;
}
