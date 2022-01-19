// Setup button/keys as defined elsewhere in the configuration
// All inputs are debounced/handled by the Bounce2 library by Thomas Fredericks which is found at:
//      https://github.com/thomasfredericks/Bounce2
//
// Defines should be configured below where...
//      COLOR_???_PIN = which hardware pin is switch connected to?
//      COLOR_???_NO = the switch is normally open when not activated
//  where COLOR is "RED" or "BLUE"

#include "HAL.h"

#include <Bounce2.h>

Bounce2::Button global_red_button = Bounce2::Button();
Bounce2::Button global_blue_button = Bounce2::Button();
Bounce2::Button global_red_key = Bounce2::Button();
Bounce2::Button global_blue_key = Bounce2::Button();

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
void core__buttons_setup() {

    // ------------------------------------------------- //
    global_red_button.attach(RED_BTN_PIN, INPUT_PULLUP);
    global_red_button.interval(DEBOUNCE_INTERVAL);
    #if defined(RED_BTN_NC)
        // RED BUTTON IS NORMALLY CLOSED
        global_red_button.setPressedState(HIGH);
    #else
        // RED BUTTON IS NORMALLY OPEN
        global_red_button.setPressedState(LOW);
    #endif

    // ------------------------------------------------- //
    global_blue_button.attach(BLUE_BTN_PIN, INPUT_PULLUP);
    global_blue_button.interval(DEBOUNCE_INTERVAL);
    #if defined(BLUE_BTN_NC)
        // BLUE BUTTON IS NORMALLY CLOSED
        global_blue_button.setPressedState(HIGH);
    #else
        // BLUE BUTTON IS NORMALLY OPEN
        global_blue_button.setPressedState(LOW);
    #endif

    // ------------------------------------------------- //
    global_red_key.attach(RED_KEY_PIN, INPUT_PULLUP);
    global_red_key.interval(DEBOUNCE_INTERVAL);
    #if defined(RED_KEY_NC)
        // RED KEY IS NORMALLY CLOSED
        global_red_key.setPressedState(HIGH);
    #else
        // RED KEY IS NORMALLY OPEN
        global_red_key.setPressedState(LOW);
    #endif

    // ------------------------------------------------- //
    global_blue_key.attach(BLUE_KEY_PIN, INPUT_PULLUP);
    global_blue_key.interval(DEBOUNCE_INTERVAL);
    #if defined(global_blue_key_NC)
        // BLUE KEY IS NORMALLY CLOSED
        global_blue_key.setPressedState(HIGH);
    #else
        // BLUE KEY IS NORMALLY OPEN
        global_blue_key.setPressedState(LOW);
    #endif
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
void core__buttons_loop() {
    // !!! DO NOT call the objects' update() functions anywhere else in the code !!!
    global_red_button.update();
    global_red_key.update();
    global_blue_button.update();
    global_blue_key.update();
}