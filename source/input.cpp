#include <cstring>

#include <padscore/kpad.h>
#include <vpad/input.h>

#include "input.hpp"


int
read_input()
{
    VPADStatus vpad_input;
    VPADReadError vpad_error;
    KPADStatus kpad_input[4];
    KPADError kpad_error;

    VPADRead(VPAD_CHAN_0, &vpad_input, 1, &vpad_error);
    
    // Initialize kpad_input to 0 since it initializes with gibberish.
    std::memset(&kpad_input, 0, sizeof(kpad_input));

    // Read all 4 KPAD channels in this loop.
    for (int channel = 0; channel < 4; channel++)
        KPADReadEx((KPADChan)channel, &kpad_input[channel], 1, &kpad_error);

    // Iterate over all 4 KPAD channels and check for button presses.
    for (int channel = 0; channel < 4; channel++) {
        // The number of buttons in each array.
        const int num_buttons = sizeof(VPAD_BUTTONS) / sizeof(VPAD_BUTTONS[0]);

        // Iterate over all buttons and check for button presses.
        for (int button_index = 0; button_index < num_buttons; button_index++) {
            if ((vpad_input.trigger & VPAD_BUTTONS[button_index]) |
                (kpad_input[channel].trigger & WPAD_BUTTONS[button_index]) |
                (kpad_input[channel].classic.trigger & WPAD_CLASSIC_BUTTONS[button_index]) |
                (kpad_input[channel].pro.trigger & WPAD_PRO_BUTTONS[button_index]))
                return VPAD_BUTTONS[button_index];
        }
    }

    return 0; // No button was pressed.
}