#include <cstring>
#include <fstream>
#include <sstream>

#include <coreinit/launch.h>
#include <coreinit/screen.h>
#include <coreinit/thread.h>
#include <coreinit/time.h>
#include <mocha/mocha.h>
#include <nn/act.h>
#include <padscore/kpad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <sndcore2/core.h>
#include <sysapp/launch.h>
#include <vpad/input.h>
#include <whb/log.h>
#include <whb/log_console.h>
#include <whb/proc.h>

#include "backup.hpp"
#include "easter_egg.hpp"
#include "input.hpp"
#include "main.hpp"
#include "start_screen.hpp"
#include "sub_screens.hpp"
#include "swap.hpp"
#include "unlink.hpp"
#include "utils.hpp"


unsigned int USER_ID; // The current user persistant ID.
bool INKAY_EXISTS; // Whether the Inkay plugin config exists.
std::string NNID_BACKUP; // The backup path to the Nintendo Network ID account.dat.
std::string PNID_BACKUP; // The backup path to the Pretendo Network ID account.dat.
std::string MII_NICKNAME; // The current user's Mii nickname.
std::string ACCOUNT_FILE; // The path to the current account.dat.
std::string INKAY_CONFIG; // The path to the Inkay configuration file.

SDL_Window* window = nullptr; // Global window variable.
SDL_Renderer* renderer = nullptr; // Global renderer variable.

int client_handle = 0; // The client handle for flushing the filesystem.


void
get_user_information()
{
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

    // Check if the Inkay configuration file exists.
    INKAY_EXISTS = std::ifstream(INKAY_CONFIG).good();
}


void
deinitialize()
{
    FSAFlushVolume(client_handle, "/vol/external01"); // Probably not necessary, but doesn't hurt.
    FSAFlushVolume(client_handle, "/vol/storage_mlc01"); // MLC actually does need to be flushed.
    FSADelClient(client_handle);

    nn::act::Finalize();
    Mocha_UnmountFS("storage_mlc");
    Mocha_DeInitLibrary();
    WHBLogConsoleFree();
    VPADShutdown();
    KPADShutdown();
    AXQuit();

    if (renderer != nullptr)
        SDL_DestroyRenderer(renderer);
    if (window != nullptr)
        SDL_DestroyWindow(window);
    
    clean_font_caches();

    TTF_Quit();
    SDL_Quit();
}


bool
initialize_program()
{
    WHBProcInit();
    VPADInit(); // Wii U GamePad support.
    KPADInit(); // External controller support.
    WPADEnableURCC(1); // Allow Pro Controller input.
    WHBLogConsoleInit(); // Initialize the log console.
    FSAInit(); // Allows for flushing the filesystem.
    AXInit(); // Disable playing audio.

    // Initialize the Mocha library.
    if (Mocha_InitLibrary() != MOCHA_RESULT_SUCCESS) {
        return false;
    }

    // Initialize the FSA handler to flush the filesystem.
    client_handle = FSAAddClient(NULL);
    if (client_handle == 0) {
        return false;
    }

    Mocha_MountFS("storage_mlc", NULL, "/vol/storage_mlc01");
    get_user_information();

    return true;
}


bool
initialize_graphics()
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    window = SDL_CreateWindow("Wii U Account Swap", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, \
        SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window)
        return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
        return false;

    return true;
}


bool
check_initialization(bool condition, const char* error_message)
{
    if (!condition) {
        WHBLogConsoleSetColor(0x99000000);
        WHBLogPrint(error_message);
        WHBLogConsoleDraw();
        OSSleepTicks(OSMillisecondsToTicks(5000));
        SYSLaunchMenu();
        return true;
    }
    return false;
}


int
main()
{
    bool error_occurred = false;

    error_occurred = check_initialization(initialize_program(), "Error initializing program!") ||
        check_initialization(initialize_graphics(), "Error initializing graphics!");

    while (WHBProcIsRunning()) {
        if (!error_occurred)
            process_screens();
    }

    deinitialize();
    WHBProcShutdown();
    return 0;
}