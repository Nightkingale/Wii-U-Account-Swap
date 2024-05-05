#include <cstring>
#include <cstdlib>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
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
#include "screen.hpp"


bool unlink_account() {
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

    // Read the entire file into a string.
    std::ifstream account_input(ACCOUNT_FILE);
    std::string file_contents((std::istreambuf_iterator<char>(account_input)), std::istreambuf_iterator<char>());
    account_input.close();

    // Process each line in the string.
    std::istringstream file_contentstream(file_contents);
    std::string line;
    while (std::getline(file_contentstream, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            if (default_values.count(key) > 0)
                line = key + "=" + default_values[key];
        }
        file_contents += line + "\n";
    }

    // Write the string back to the file.
    std::ofstream account_output(ACCOUNT_FILE);
    account_output << file_contents;
    account_output.close();

    draw_success_menu("unlink");
    OSEnableHomeButtonMenu(1);

    return true;
}
