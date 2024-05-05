#include <cstring>
#include <sstream>

#include <coreinit/screen.h>
#include <coreinit/thread.h>
#include <coreinit/time.h>
#include <mocha/mocha.h>
#include <nn/act.h>
#include <padscore/kpad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <sndcore2/core.h>
#include <vpad/input.h>
#include <whb/log.h>
#include <whb/log_console.h>
#include <whb/proc.h>

#include "backup.hpp"
#include "input.hpp"
#include "main.hpp"
#include "screen.hpp"
#include "switch.hpp"
#include "unlink.hpp"


unsigned int USER_ID; // The current user persistant ID.
std::string NNID_BACKUP; // The backup path to the Nintendo Network ID account.dat.
std::string PNID_BACKUP; // The backup path to the Pretendo Network ID account.dat.
std::string MII_NICKNAME; // The current user's Mii nickname.
std::string ACCOUNT_FILE; // The path to the current account.dat.
std::string INKAY_CONFIG; // The path to the Inkay configuration file.

SDL_Window* window = nullptr; // Global window variable.
SDL_Renderer* renderer = nullptr; // Global renderer variable.


void get_user_information() {
    // Grab the user's Mii name and persistent ID.
    nn::act::Initialize();
    int16_t mii_name[256];
    nn::act::GetMiiName(mii_name);
    MII_NICKNAME = std::string(mii_name, mii_name + sizeof(mii_name) / sizeof(mii_name[0]));
    USER_ID = nn::act::GetPersistentId();

    // Set the account file path.
    char user_id_hex[9];
    sprintf(user_id_hex, "%08x", USER_ID);
    ACCOUNT_FILE = "storage_mlc:/usr/save/system/act/" + std::string(user_id_hex) + "/account.dat";

    // Set the backup file paths.
    NNID_BACKUP = "fs:/vol/external01/wiiu/accounts/" + std::string(user_id_hex) + "/nnid_account.dat";
    PNID_BACKUP = "fs:/vol/external01/wiiu/accounts/" + std::string(user_id_hex) + "/pnid_account.dat";

    // Set the Inkay configuration file path.
    char environment_path_buffer[0x100];
    Mocha_GetEnvironmentPath(environment_path_buffer, sizeof(environment_path_buffer));
    INKAY_CONFIG = std::string(environment_path_buffer) + std::string("/plugins/config/inkay.json");
}


void deinitialize() {
    // This prevents hangs when called twice.
    static bool is_deinitialized = false;

    if (!is_deinitialized) {
        nn::act::Finalize();
        Mocha_UnmountFS("storage_mlc");
        Mocha_DeInitLibrary();
        WHBLogConsoleFree();
        VPADShutdown();
        KPADShutdown();

        if (renderer != nullptr)
            SDL_DestroyRenderer(renderer);
        if (window != nullptr)
            SDL_DestroyWindow(window);
        is_deinitialized = true;
    }
}


void initialize_program() {
    WHBProcInit();
    VPADInit();
    KPADInit();
    AXInit();
    AXQuit();
    WPADEnableURCC(1);

    // Set up the log console for use.
    WHBLogConsoleInit();
    WHBLogConsoleSetColor(0x00009900);

    // Initialize the Mocha library.
    if (Mocha_InitLibrary() != MOCHA_RESULT_SUCCESS) {
        WHBLogConsoleSetColor(0x99000000);
        WHBLogPrint("Mocha_InitLibrary failed!");
        WHBLogConsoleDraw();
        OSSleepTicks(OSMillisecondsToTicks(5000));
        deinitialize();
    }

    // Mount the storage device differently depending on Tiramisu or Aroma.
    Mocha_MountFS("storage_mlc", NULL, "/vol/storage_mlc01");
    get_user_information();
}


bool initialize_graphics() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    const int SCREEN_WIDTH = 1920;
    const int SCREEN_HEIGHT = 1080;

    window = SDL_CreateWindow("Wii U Account Swap", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (window == NULL)
        return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        return false;

    return true;
}


int main() {
    bool is_running = true;
    initialize_program();

    if (initialize_graphics()) {
        WHBLogConsoleSetColor(0x99000000);
        WHBLogPrint("Failed to initialize SDL2!");
        WHBLogConsoleDraw();
        OSSleepTicks(OSMillisecondsToTicks(5000));
        is_running = false;
    }

    int selected_option = 0;
    const int NUM_OPTIONS = 4;
    OSEnableHomeButtonMenu(1);

    while (is_running) {
        draw_menu_screen(selected_option);

        int button = read_input(); // Watch the controllers for input.

        if (button == VPAD_BUTTON_UP) {
            selected_option--;
            if (selected_option < 0) {
                selected_option = NUM_OPTIONS - 1;
            }
        } else if (button == VPAD_BUTTON_DOWN) {
            selected_option++;
            if (selected_option >= NUM_OPTIONS) {
                selected_option = 0;
            }
        } else if (button == VPAD_BUTTON_A) {
            switch (selected_option) {
                case 0:
                    switch_account(NNID_BACKUP.c_str(), "Nintendo Network ID");
                    break;
                case 1:
                    switch_account(PNID_BACKUP.c_str(), "Pretendo Network ID");
                    break;
                case 2:
                    while (is_running) {
                        draw_backup_menu();
                        button = read_input();

                        if (button == VPAD_BUTTON_A) {
                            break;
                        } else if (button == VPAD_BUTTON_B) {
                            break;
                        }
                    }
                    break;
                case 3:
                    while (is_running) {
                        draw_unlink_menu();
                        button = read_input();

                        if (button == VPAD_BUTTON_A) {
                            break;
                        } else if (button == VPAD_BUTTON_B) {
                            break;
                        }
                    }
                    break;
            }
        }
    }

    deinitialize();
    SDL_Quit();

    return 0;
}