#include <cstring>

#include <padscore/kpad.h>
#include <vpad/input.h>


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

    // Wii U GamePad buttons.
    const int vpad_buttons[] = {
        VPAD_BUTTON_A,
        VPAD_BUTTON_B,
        VPAD_BUTTON_X,
        VPAD_BUTTON_Y,
        VPAD_BUTTON_LEFT,
        VPAD_BUTTON_RIGHT,
        VPAD_BUTTON_UP,
        VPAD_BUTTON_DOWN,
        VPAD_BUTTON_L,
        VPAD_BUTTON_R,
        VPAD_BUTTON_ZL,
        VPAD_BUTTON_ZR,
        VPAD_BUTTON_PLUS,
        VPAD_BUTTON_MINUS,
        VPAD_BUTTON_STICK_L,
        VPAD_BUTTON_STICK_R,
        VPAD_BUTTON_HOME,
        VPAD_BUTTON_TV,
        VPAD_BUTTON_SYNC
    };

    // Wii Remote buttons.
    const int wpad_buttons[] = {
        WPAD_BUTTON_A,
        WPAD_BUTTON_B,
        WPAD_BUTTON_1,
        WPAD_BUTTON_2,
        WPAD_BUTTON_LEFT,
        WPAD_BUTTON_RIGHT,
        WPAD_BUTTON_UP,
        WPAD_BUTTON_DOWN,
        0,
        0,
        0,
        0,
        WPAD_BUTTON_PLUS,
        WPAD_BUTTON_MINUS,
        0,
        0,
        WPAD_BUTTON_HOME,
        0,
        0
    };

    // Wii Classic Controller buttons.
    const int wpad_classic_buttons[] = {
        WPAD_CLASSIC_BUTTON_A,
        WPAD_CLASSIC_BUTTON_B,
        WPAD_CLASSIC_BUTTON_X,
        WPAD_CLASSIC_BUTTON_Y,
        WPAD_CLASSIC_BUTTON_LEFT,
        WPAD_CLASSIC_BUTTON_RIGHT,
        WPAD_CLASSIC_BUTTON_UP,
        WPAD_CLASSIC_BUTTON_DOWN,
        WPAD_CLASSIC_BUTTON_L,
        WPAD_CLASSIC_BUTTON_R,
        WPAD_CLASSIC_BUTTON_ZL,
        WPAD_CLASSIC_BUTTON_ZR,
        WPAD_CLASSIC_BUTTON_PLUS,
        WPAD_CLASSIC_BUTTON_MINUS,
        0,
        0,
        WPAD_CLASSIC_BUTTON_HOME,
        0,
        0
    };

    // Wii U Pro Controller buttons.
    const int wpad_pro_buttons[] = {
        WPAD_PRO_BUTTON_A,
        WPAD_PRO_BUTTON_B,
        WPAD_PRO_BUTTON_X,
        WPAD_PRO_BUTTON_Y,
        WPAD_PRO_BUTTON_LEFT,
        WPAD_PRO_BUTTON_RIGHT,
        WPAD_PRO_BUTTON_UP,
        WPAD_PRO_BUTTON_DOWN,
        WPAD_PRO_TRIGGER_L,
        WPAD_PRO_TRIGGER_R,
        WPAD_PRO_TRIGGER_ZL,
        WPAD_PRO_TRIGGER_ZR,
        WPAD_PRO_BUTTON_PLUS,
        WPAD_PRO_BUTTON_MINUS,
        WPAD_PRO_BUTTON_STICK_L,
        WPAD_PRO_BUTTON_STICK_R,
        WPAD_PRO_BUTTON_HOME,
        0,
        0
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