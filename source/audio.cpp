#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "nightkingale_ogg.h"


bool play_easter_egg() {
    // Initialize SDL_mixer.
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        return false;
    }

    // Load the OGG file.
    SDL_RWops *rw = SDL_RWFromConstMem(nightkingale_ogg, nightkingale_ogg_size);
    Mix_Music *music = Mix_LoadMUS_RW(rw, 1);

    // Play the music once.
    Mix_PlayMusic(music, 1);

    // Wait for the music to finish.
    while (Mix_PlayingMusic()) {
        SDL_Delay(100);
    }

    // Clean up.
    Mix_FreeMusic(music);
    Mix_CloseAudio();

    return true;
}