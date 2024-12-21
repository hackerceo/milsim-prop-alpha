
// ============== Languages ===============
unsigned char   global_LANGUAGE = 0;
[[LANGUAGE_DEFINES]]
[[LANGUAGE_CODES]]

// ============== Game Modes ==============
unsigned char   global_GAME_MODE = 0;
[[GAME_MODES]]

// ============== MP3 Folders ==============
//  global_MP3_FOLDER[game_mode][language]
//  ...a value of 0 means that there is no MP3
// =========================================
[[MP3_FOLDER_ARRAY]]

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
void setup() {
[[SETUP]]
}

// This is the main loop function that dispatches execution to the proper game
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
void loop() {
    core__loop();
    switch(global_GAME_MODE) {
        case GAME_MODE_core:
            break;
[[LOOP_CASES]]
        default:
            // we should NEVER have a bad game mode
            ESP.restart();
    }
}