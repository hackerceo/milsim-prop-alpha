#define domination__EVENT_BUTTON_RELEASED    0
#define domination__EVENT_BACK_TO_IDLE       1
#define domination__EVENT_GAME_OVER          2

// game variables
long domination__TIMER_GAME;
long domination__TIMER_RED;
long domination__TIMER_BLUE;
long domination__TIMER_ACCUMULATOR;
 int domination__LAST_SECONDS;
 int domination__LAST_TENTH_SEC;

// config variables
long domination__GAME_TIMER_END;
long domination__TEAM_TIMER_END;
bool domination__USE_GAME_TIMER;
bool domination__USE_TEAM_TIMER;
bool domination__KEEP_DEPRESSED;

extern Fsm domination__fsm;

// =================================================================
State domination__state_Idle(
    [](){
        Serial.println("-> STATE: domination:Idle");
        domination__TIMER_ACCUMULATOR = millis();
    },
    [](){
        long duration = millis() - domination__TIMER_ACCUMULATOR;
        // see if the game timer has ended
        if (domination__USE_GAME_TIMER) {
            if ((domination__TIMER_GAME + duration) >= domination__GAME_TIMER_END) domination__fsm.trigger(domination__EVENT_GAME_OVER);
        }
        // update the display (once per second)
        if (domination__LAST_SECONDS != int (duration/1000)) {
            // update the game timer
            domination__LAST_SECONDS = duration/1000;
            // TODO: update game clock
        }
    },
    [](){
        Serial.println("<- STATE: domination:Idle");
        domination__TIMER_GAME = (millis() - domination__TIMER_ACCUMULATOR) + domination__TIMER_GAME;
    }
);
// =================================================================
State domination__state_CapturingRed(
    [](){
        Serial.println("-> STATE: domination:CapturingRed");
        domination__TIMER_ACCUMULATOR = millis();
        // TODO: play capturing sound
    },
    [](){
        long duration = millis() - domination__TIMER_ACCUMULATOR;
        // see if the game timer has ended
        if (domination__USE_GAME_TIMER) {
            if ((domination__TIMER_GAME + duration) >= domination__GAME_TIMER_END) domination__fsm.trigger(domination__EVENT_GAME_OVER);
        }
        // if button is released then go back to Idle state
        if (global_red_button.released()) {
            domination__fsm.trigger(domination__EVENT_BUTTON_RELEASED);
        }
        // update the display (once per second)
        if (domination__LAST_SECONDS != int (duration/1000)) {
            // update the game timer
            domination__LAST_SECONDS = duration/1000;
            // TODO: update game clock
        }
        if (domination__LAST_TENTH_SEC != int (duration/100)) {
            // TODO: update capture bar display
            domination__LAST_TENTH_SEC = int (duration/100);
        }
    },
    [](){
        Serial.println("<- STATE: domination:CapturingRed");
        domination__TIMER_GAME = (millis() - domination__TIMER_ACCUMULATOR) + domination__TIMER_GAME;
    }
);
// =================================================================
State domination__state_CapturingBlue(
    [](){
        Serial.println("-> STATE: domination:CapturingBlue");
        domination__TIMER_ACCUMULATOR = millis();
        // TODO: play capturing sound
    },
    [](){
        long duration = millis() - domination__TIMER_ACCUMULATOR;
        // see if the game timer has ended
        if (domination__USE_GAME_TIMER) {
            if ((domination__TIMER_GAME + duration) >= domination__GAME_TIMER_END) domination__fsm.trigger(domination__EVENT_GAME_OVER);
        }
        // if button is released then go back to Idle state
        if (global_blue_button.released()) {
            domination__fsm.trigger(domination__EVENT_BUTTON_RELEASED);
        }
        // update the display (once per second)
        if (domination__LAST_SECONDS != int (duration/1000)) {
            // update the game timer
            domination__LAST_SECONDS = duration/1000;
            // TODO: update game clock
        }
        if (domination__LAST_TENTH_SEC != int (duration/100)) {
            // TODO: update capture bar display
            domination__LAST_TENTH_SEC = int (duration/100);
        }
    },
    [](){
        Serial.println("<- STATE: domination:CapturingBlue");
        domination__TIMER_GAME = (millis() - domination__TIMER_ACCUMULATOR) + domination__TIMER_GAME;
    }
);
// =================================================================
State domination__state_CapturedRed(
    [](){
        Serial.println("-> STATE: domination:CapturedRed");
        domination__TIMER_ACCUMULATOR = millis();
        // TODO: play captured sound
    },
    [](){
        long duration = millis() - domination__TIMER_ACCUMULATOR;
        // see if the game timer has ended
        if (domination__USE_GAME_TIMER) {
            if ((domination__TIMER_GAME + duration) >= domination__GAME_TIMER_END) domination__fsm.trigger(domination__EVENT_GAME_OVER);
        }
        // see if the team timer has ended
        if (domination__USE_TEAM_TIMER) {
            if ((domination__TIMER_RED + duration) >= domination__TEAM_TIMER_END) domination__fsm.trigger(domination__EVENT_GAME_OVER);
        }
        // see if button release caused flag forfeit (if configured)
        if (domination__KEEP_DEPRESSED) {
            if (global_red_button.released()) domination__fsm.trigger(domination__EVENT_BACK_TO_IDLE);
        }
        // update the display (once per second)
        if (domination__LAST_SECONDS != int (duration/1000)) {
            // update the game timer
            domination__LAST_SECONDS = duration/1000;
            // TODO: update game clock
            // TODO: update red team clock
        }
    },
    [](){
        Serial.println("<- STATE: domination:CapturedRed");
        long duration = (millis() - domination__TIMER_ACCUMULATOR)
        domination__TIMER_GAME = domination__TIMER_GAME + duration;
        domination__TIMER_RED = domination__TIMER_RED + duration;
    }
);
// =================================================================
State domination__state_CapturedBlue(
    [](){
        Serial.println("-> STATE: domination:CapturedBlue");
        domination__TIMER_ACCUMULATOR = millis();
        // TODO: play captured sound
    },
    [](){
        long duration = (millis() - domination__TIMER_ACCUMULATOR);
        // see if the game timer has ended
        if (domination__USE_GAME_TIMER) {
            if ((domination__TIMER_GAME + duration) >= domination__GAME_TIMER_END) domination__fsm.trigger(domination__EVENT_GAME_OVER);
        }
        // see if the team timer has ended
        if (domination__USE_TEAM_TIMER) {
            if ((domination__TIMER_BLUE + duration) >= domination__TEAM_TIMER_END) domination__fsm.trigger(domination__EVENT_GAME_OVER);
        }
        // see if button release caused flag forfeit (if configured)
        if (domination__KEEP_DEPRESSED) {
            if (global_blue_button.released()) domination__fsm.trigger(domination__EVENT_BACK_TO_IDLE);
        }
        // update the display (once per second)
        if (domination__LAST_SECONDS != int (duration/1000)) {
            // update the game timer
            domination__LAST_SECONDS = duration/1000;
            // TODO: update game clock
            // TODO: update blue team clock
        }
    },
    [](){
        Serial.println("<- STATE: domination:CapturedBlue");
        long duration = (millis() - domination__TIMER_ACCUMULATOR)
        domination__TIMER_GAME = domination__TIMER_GAME + duration;
        domination__TIMER_BLUE = domination__TIMER_BLUE + duration;
    }
);
// =================================================================
State domination__state_RecaptureRed(
    [](){
        Serial.println("-> STATE: domination:RecaptureRed");
        domination__TIMER_ACCUMULATOR = millis();
        // TODO: play capturing sound
    },
    [](){
        long duration = millis() - domination__TIMER_ACCUMULATOR;
        // see if the game timer has ended
        if (domination__USE_GAME_TIMER) {
            if ((domination__TIMER_GAME + duration) >= domination__GAME_TIMER_END) domination__fsm.trigger(domination__EVENT_GAME_OVER);
        }
        // if button is released then go back to CapturedBlue state
        if (global_red_button.released()) {
            domination__fsm.trigger(domination__EVENT_BUTTON_RELEASED);
        }
        // update the display (once per second)
        if (domination__LAST_SECONDS != int (duration/1000)) {
            // update the game timer
            domination__LAST_SECONDS = duration/1000;
            // TODO: update game clock
        }
        if (domination__LAST_TENTH_SEC != int (duration/100)) {
            // TODO: update capture bar display
            domination__LAST_TENTH_SEC = int (duration/100);
        }
    },
    [](){
        Serial.println("<- STATE: domination:RecaptureRed");
        long duration = (millis() - domination__TIMER_ACCUMULATOR)
        domination__TIMER_GAME = domination__TIMER_GAME + duration;
    }
);
// =================================================================
State domination__state_RecaptureBlue(
    [](){
        Serial.println("-> STATE: domination:RecaptureBlue");
        domination__TIMER_ACCUMULATOR = millis();
        // TODO: play capturing sound
    },
    [](){
        long duration = millis() - domination__TIMER_ACCUMULATOR;
        // see if the game timer has ended
        if (domination__USE_GAME_TIMER) {
            if ((domination__TIMER_GAME + duration) >= domination__GAME_TIMER_END) domination__fsm.trigger(domination__EVENT_GAME_OVER);
        }
        // if button is released then go back to CapturedRed state
        if (global_blue_button.released()) {
            domination__fsm.trigger(domination__EVENT_BUTTON_RELEASED);
        }
        // update the display (once per second)
        if (domination__LAST_SECONDS != int (duration/1000)) {
            // update the game timer
            domination__LAST_SECONDS = duration/1000;
            // TODO: update game clock
        }
        if (domination__LAST_TENTH_SEC != int (duration/100)) {
            // TODO: update capture bar display
            domination__LAST_TENTH_SEC = int (duration/100);
        }
    },
    [](){
        Serial.println("<- STATE: domination:RecaptureBlue");
        long duration = (millis() - domination__TIMER_ACCUMULATOR)
        domination__TIMER_GAME = domination__TIMER_GAME + duration;
    }
);
// =================================================================
State domination__state_GameOver(
    [](){
        Serial.println("-> STATE: domination:GameOver");
        // TODO: play game over sound(s)
    },
    NULL,
    NULL
);
// =================================================================


Fsm domination__fsm(&domination__state_Idle);


// -----------------------------------------------------------------
Fsm domination__setup() {
    // set timers to zero
    domination__TIMER_GAME = 0;
    domination__TIMER_RED = 0;
    domination__TIMER_BLUE = 0;

    // TODO: load the settings from config json
    // domination__GAME_TIMER_END = ;
    // domination__TEAM_TIMER_END = ;
    // domination__USE_GAME_TIMER = ;
    // domination__USE_TEAM_TIMER = ;
    // domination__KEEP_DEPRESSED = ;

    // ########## Create FSM transitions ##########
    // first capture via button press
    core__fsm.add_transition(&domination__state_Idle, &domination__state_CapturingRed, GLOBAL_EVENT_RED_BUTTON_PRESS, NULL);
    core__fsm.add_transition(&domination__state_Idle, &domination__state_CapturingBlue, GLOBAL_EVENT_BLUE_BUTTON_PRESS, NULL);

    // from capturing state back to idle if the button is released prematurely
    core__fsm.add_transition(&domination__state_CapturingRed, &domination__state_Idle, domination__EVENT_BUTTON_RELEASED, &domination__playsound_aborted);
    core__fsm.add_transition(&domination__state_CapturingBlue, &domination__state_Idle, domination__EVENT_BUTTON_RELEASED, &domination__playsound_aborted);

    // capture flag when capture timer expires without
    core__fsm.add_timed_transition(&domination__state_CapturingRed, &domination__state_CapturedRed, 10000, NULL);
    core__fsm.add_timed_transition(&domination__state_CapturingBlue, &domination__state_CapturedBlue, 10000, NULL);

    // recapturing when an opposite team button is pressed
    core__fsm.add_transition(&domination__state_CapturedRed, &domination__state_RecaptureBlue, GLOBAL_EVENT_BLUE_BUTTON_PRESS, NULL);
    core__fsm.add_transition(&domination__state_CapturedBlue, &domination__state_RecaptureRed, GLOBAL_EVENT_RED_BUTTON_PRESS, NULL);

    // forfeit flag possession when the captured team's button is released
    core__fsm.add_transition(&domination__state_CapturedRed, &domination__state_Idle, domination__EVENT_BACK_TO_IDLE, &domination__playsound_aborted);
    core__fsm.add_transition(&domination__state_CapturedBlue, &domination__state_Idle, domination__EVENT_BACK_TO_IDLE, &domination__playsound_aborted);

    // change back to previous capture state if the button is released prematurely
    core__fsm.add_transition(&domination__state_RecaptureRed, &domination__state_CapturedBlue, domination__EVENT_BUTTON_RELEASED, &domination__playsound_aborted);
    core__fsm.add_transition(&domination__state_RecaptureBlue, &domination__state_CapturedRed, domination__EVENT_BUTTON_RELEASED, &domination__playsound_aborted);

    // capture flag when capture timer expires without
    core__fsm.add_timed_transition(&domination__state_RecaptureRed, &domination__state_CapturedRed, 10000, NULL);
    core__fsm.add_timed_transition(&domination__state_RecaptureBlue, &domination__state_CapturedBlue, 10000, NULL);


    core__fsm.add_transition(&domination__state_CapturedRed, &domination__state_RecaptureBlue, GLOBAL_EVENT_BLUE_BUTTON_PRESS, NULL);
    core__fsm.add_transition(&domination__state_CapturedBlue, &domination__state_RecaptureRed, GLOBAL_EVENT_RED_BUTTON_PRESS, NULL);

    // game over transitions
    core__fsm.add_transition(&domination__state_Idle, &domination__state_GameOver, domination__EVENT_GAME_OVER, NULL);
    core__fsm.add_transition(&domination__state_CapturingRed, &domination__state_GameOver, domination__EVENT_GAME_OVER, NULL);
    core__fsm.add_transition(&domination__state_CapturingBlue, &domination__state_GameOver, domination__EVENT_GAME_OVER, NULL);
    core__fsm.add_transition(&domination__state_CapturedRed, &domination__state_GameOver, domination__EVENT_GAME_OVER, NULL);
    core__fsm.add_transition(&domination__state_CapturedBlue, &domination__state_GameOver, domination__EVENT_GAME_OVER, NULL);
    core__fsm.add_transition(&domination__state_RecaptureRed, &domination__state_GameOver, domination__EVENT_GAME_OVER, NULL);
    core__fsm.add_transition(&domination__state_RecaptureBlue, &domination__state_GameOver, domination__EVENT_GAME_OVER, NULL);

    return &domination__fsm;
}
// -----------------------------------------------------------------

void domination__playsound_aborted() {
    // TODO: play aborted sound
}