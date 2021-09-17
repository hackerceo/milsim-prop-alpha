
// Buttons and keys
#define DEBOUNCE_INTERVAL 20    // set debounce interval to 20ms
#define RED_BTN_PIN     34
#define RED_KEY_PIN     39
#define BLUE_BTN_PIN    35
#define BLUE_KEY_PIN    36
#define BLUE_BTN_NO     // Industrial Blue buttons are normally open
#define RED_BTN_NO      // ...for breadboard prototype testing
//#define RED_BTN_NC      // Industrial Red buttons are normally closed
#define BLUE_KEY_NO     // Key switches are normally open ...
#define RED_KEY_NO      // ...


// Pins used by the Blue timer
#define PIN_TMR_BLUE_CLK	4
#define PIN_TMR_BLUE_DATA	13

// Pins used by the Red timer
#define PIN_TMR_RED_CLK		33
#define PIN_TMR_RED_DATA    32

// Serial used by the DF Player module
#define SERIAL_MP3			Serial2

// Define I2C Addresses
#define I2C_ADDRESS_MASTER	0xef
#define I2C_ADDRESS_LCD		0x3f
#define I2C_ADDRESS_WIRECUT 0x39
#define I2C_ADDRESS_UART    0x3a
#define I2C_ADDRESS_KEYPAD  0x3b
