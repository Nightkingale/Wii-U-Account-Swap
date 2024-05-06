#include <cstring>

#include <padscore/kpad.h>
#include <vpad/input.h>


int read_input() {
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

    // Wii U GamePad buttons.
    const int vpad_buttons[] = {
        VPAD_BUTTON_A,
        VPAD_BUTTON_B,
        VPAD_BUTTON_UP,
        VPAD_BUTTON_DOWN,
        VPAD_BUTTON_SYNC
    };

    // Wii Remote buttons.
    const int wpad_buttons[] = {
        WPAD_BUTTON_A,
        WPAD_BUTTON_B,
        WPAD_BUTTON_UP,
        WPAD_BUTTON_DOWN,
        0 // WPAD_BUTTON_SYNC does not exist.
    };

    // Wii Classic Controller buttons.
    const int wpad_classic_buttons[] = {
        WPAD_CLASSIC_BUTTON_A,
        WPAD_CLASSIC_BUTTON_B,
        WPAD_CLASSIC_BUTTON_UP,
        WPAD_CLASSIC_BUTTON_DOWN,
        0 // WPAD_CLASSIC_BUTTON_SYNC does not exist.
    };

    // Wii U Pro Controller buttons.
    const int wpad_pro_buttons[] = {
        WPAD_PRO_BUTTON_A,
        WPAD_PRO_BUTTON_B,
        WPAD_PRO_BUTTON_UP,
        WPAD_PRO_BUTTON_DOWN,
        0 // WPAD_PRO_BUTTON_SYNC does not exist.
    };

    // Iterate over all 4 KPAD channels and check for button presses.
    for (int channel = 0; channel < 4; channel++) {
        // The number of buttons in each array.
        const int num_buttons = sizeof(vpad_buttons) / sizeof(vpad_buttons[0]);

        // Iterate over all buttons and check for button presses.
        for (int button_index = 0; button_index < num_buttons; button_index++) {
            if ((vpad_input.trigger & vpad_buttons[button_index]) |
                (kpad_input[channel].trigger & wpad_buttons[button_index]) |
                (kpad_input[channel].classic.trigger & wpad_classic_buttons[button_index]) |
                (kpad_input[channel].pro.trigger & wpad_pro_buttons[button_index]))
                return vpad_buttons[button_index];
        }
    }

    return 0; // No button was pressed.
}