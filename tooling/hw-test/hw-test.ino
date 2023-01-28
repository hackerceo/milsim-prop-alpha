//#include <deprecated.h>
//#include <MFRC522.h>
//#include <MFRC522Extended.h>
//#include <require_cpp11.h>

#include <AsyncEventSource.h>
#include <AsyncJson.h>
#include <AsyncWebSocket.h>
#include <AsyncWebSynchronization.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>
#include <StringArray.h>
#include <WebAuthentication.h>
#include <WebHandlerImpl.h>
#include <WebResponseImpl.h>
#include <AsyncTCP.h>
#include <SPI.h>
#include <SPIFFS.h>
#include <Fsm.h>


// Hardware Abstraction Layer file
// this defines all your pins/addresses
#include "HAL.h"

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel LedStrip(24, PIN_NEOPIXELS, NEO_GRB + NEO_KHZ800);

#include <Wire.h>
#include <ADXL345.h>
ADXL345 accelerometer;


#include <I2CRC522.h>         //  and replace MFRC522 with I2CRC522 
I2CRC522 mfrc522(I2C_RFID_RST, I2C_ADDRESS_RFID);  // Create MFRC522 instance


#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>

#include <TM1637TinyDisplay.h>
TM1637TinyDisplay    tmr_Red(PIN_TMR_RED_CLK, PIN_TMR_RED_DATA);
TM1637TinyDisplay    tmr_Blue(PIN_TMR_BLUE_CLK, PIN_TMR_BLUE_DATA);

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(I2C_ADDRESS_LCD,20,4);

#include <DFPlay.h>
DFPlay mp3Player;

#include <Bounce2.h>
Bounce2::Button global_red_button = Bounce2::Button();
Bounce2::Button global_blue_button = Bounce2::Button();
Bounce2::Button global_red_key = Bounce2::Button();
Bounce2::Button global_blue_key = Bounce2::Button();

unsigned long myTime;
typedef struct {
  uint8_t   pipe_id;
  uint16_t  device_id;
  uint8_t   message_num;
  uint8_t   control_bits;
  uint8_t   message_type_id;
  uint8_t   message[240];
} struct_ESPNOW_msg;
struct_ESPNOW_msg TestMessage;

long unsigned int myTimerStart;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Serial started...");  

  Serial.println("=== Setting up Mesh WiFi...");
  uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  esp_now_peer_info_t nowBcast;
  nowBcast.channel = 1;
  nowBcast.encrypt = 0;
  memcpy(nowBcast.peer_addr, broadcastAddress, sizeof(broadcastAddress));
  WiFi.mode(WIFI_STA);
  esp_wifi_set_protocol(ESP_IF_WIFI_STA, WIFI_PROTOCOL_LR); // <---- THIS ENABLES LONG-RANGE TRANSMIT!!!
  Serial.print("Mac Address in Station: ");
  Serial.println(WiFi.macAddress());
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  } else {
    Serial.println("ESPNow Init Failed");
    ESP.restart();
  }
  esp_err_t result = esp_now_add_peer(&nowBcast);
  if (result != ESP_OK){
    Serial.printf("Failed to add broadcast peer: %s\n", esp_err_to_name(result));
    ESP.restart();
    return;
  }
  esp_now_register_send_cb([](const uint8_t *mac_addr, esp_now_send_status_t status){
    Serial.print("Time to send: ");
    Serial.print(millis() - myTimerStart);
    Serial.println(" ms");
    Serial.print("Last Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
    return;
  });
  TestMessage.pipe_id = 0;
  TestMessage.device_id = 0xffff;
  TestMessage.message_num = 0;
  TestMessage.control_bits = 0x00;
  TestMessage.message_type_id = 0x00;
  memcpy(&TestMessage.message, "This is a test message!", 23);
  myTimerStart = millis();
  result = esp_now_send(broadcastAddress, (uint8_t *) &TestMessage, (6 + 23));
  if (result != ESP_OK) {
    Serial.printf("Failed to send: %s\n", esp_err_to_name(result));
  }
  delay(1000);


// ==== test sending second message
  if (esp_wifi_start() == ESP_OK) {
    Serial.println("WIFI Init Success");
  } else {
    Serial.println("WIFI Init Failed");
  }
  result = esp_now_send(broadcastAddress, (uint8_t *) &TestMessage, (6 + 23));
  if (result != ESP_OK) {
    Serial.printf("Failed to send: %s\n", esp_err_to_name(result));
  }
  delay(100);
  esp_wifi_stop();
  Serial.println("WiFi disconnected...");
  delay(1000);
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  Serial.println("=== Setting up MP3 Serial...");
  SERIAL_MP3.begin(9600);          // initializes the Serial connection @ 9600 baud
  mp3Player.begin(SERIAL_MP3);     // Prepares DFPlay for execution  

  Serial.println("=== Setting up LCD...");
  lcd.init();                   // initialize the lcd 

  // Print base message to the LCD
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("Brooklyn Tactical");
  lcd.setCursor(3,1);
  lcd.print("[Hardware Test]");

  Serial.println("=== Init up Timers...");
  tmr_Blue.setBrightness(0x0F); 
  tmr_Red.setBrightness(0x0F); 
  tmr_Blue.showString("BLUE"); 
  tmr_Red.showString("RED");
  delay(1000);

  Serial.println("=== Init Neopixels...");
  lcd.setCursor(0,2);
  lcd.print("   ...LED Bars...   ");
  LedStrip.begin();
  LedStrip.show();
  for(int i=0; i<LedStrip.numPixels(); i++) {
    LedStrip.setPixelColor(i, LedStrip.Color(255, 0, 0));
  }
  LedStrip.show();
  delay(1000);
  for(int i=0; i<LedStrip.numPixels(); i++) {
    LedStrip.setPixelColor(i, LedStrip.Color(0, 255, 0));
  }
  LedStrip.show();
  delay(1000);
  for(int i=0; i<LedStrip.numPixels(); i++) {
    LedStrip.setPixelColor(i, LedStrip.Color(0, 0, 255));
  }
  LedStrip.show();
  delay(1000);
  for(int i=0; i<8; i++) {
    LedStrip.setPixelColor(i, LedStrip.Color(16, 0, 0));
  }
  for(int i=8; i<16; i++) {
    LedStrip.setPixelColor(i, LedStrip.Color(0, 0, 0));
  }
  for(int i=16; i<24; i++) {
    LedStrip.setPixelColor(i, LedStrip.Color(0, 0, 16));
  }
  LedStrip.show();


  Serial.println("=== Setting up Buttons...");
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


  // === Test Battery ===
  Serial.println("=== Battery...");
  lcd.setCursor(0,2);
  lcd.print("   ...Battery...    ");
  #ifdef BATTERY_DISABLES_WIFI
    esp_wifi_stop();
    Serial.println("WiFi disconnected...");
  #endif
  int batteryLevel = 0;  
  lcd.setCursor(0,3);
  lcd.print("Level:              ");
  for (byte i=0; i<10; i++) {
    batteryLevel = analogRead(PIN_BATTERY);
    Serial.print("Battery Level: ");
    Serial.println(batteryLevel);
    lcd.setCursor(7,3);
    lcd.print(batteryLevel);
    delay(250);
  }
  #ifdef BATTERY_DISABLES_WIFI
    if (esp_wifi_start() == ESP_OK) {
      Serial.println("WIFI restarted");
    } else {
      Serial.println("WIFI restart failed");
    }
  #endif
  delay(2000);



  /// === Test Buttons ===
  lcd.setCursor(0,2);
  lcd.print("    ...Buttons...   ");

  lcd.setCursor(0,3);
  lcd.print("  Press Red Btn  ");
  while(!global_red_button.isPressed()) {
    delay(25);
    global_red_button.update();
  }
  Serial.println("  Red button pressed");
  lcd.setCursor(0,3);
  lcd.print("  Red Btn Pressed!  ");
  delay(3000);  

  lcd.setCursor(0,3);
  lcd.print("  Press Blue Btn ");
  while(!global_blue_button.isPressed()) {
    delay(25);
    global_blue_button.update();
  }
  Serial.println("  Blue button pressed");
  lcd.setCursor(0,3);
  lcd.print(" Blue Btn Pressed!  ");
  delay(3000);  



  /// === Test Keys ===
  Serial.println("=== Setting up Keys...");
  lcd.setCursor(0,2);
  lcd.print("     ...Keys...     ");

  lcd.setCursor(0,3);
  lcd.print("    Turn Red Key    ");
  while(!global_red_key.isPressed()) {
    delay(25);
    global_red_key.update();
  }
  Serial.println("  Red key activated");
  lcd.setCursor(0,3);
  lcd.print(" Red Key Activated! ");
  delay(3000);  

  Serial.println("  Blue key activated");
  lcd.setCursor(0,3);
  lcd.print("    Turn Blue Key   ");
  while(!global_blue_key.isPressed()) {
    delay(25);
    global_blue_key.update();
  }
  lcd.setCursor(0,3);
  lcd.print(" Blue Key Activated!");
  delay(3000);  



  // === Test Timers ===
  Serial.println("=== Testing Timers... (brightness increment 0-15)");
  lcd.setCursor(0,2);
  lcd.print("    ...Timers...    ");
  lcd.setCursor(0,3);
  lcd.print("Brightness =        ");
  for (int bright=0; bright < 16; bright++) {
    Serial.print(bright, HEX);
    lcd.setCursor(13,3);
    lcd.print(String(bright,HEX));
    tmr_Blue.setBrightness(bright); 
    tmr_Red.setBrightness(bright); 
    tmr_Blue.showNumber(bright); 
    tmr_Red.showNumber(bright); 
    delay(500);
  }
  tmr_Blue.showString("BLUE");                
  tmr_Red.showString("RED");                
  Serial.println("...DONE!");


  
  // === Test MP3 ===
  Serial.print("=== Playing MP3 (3 secs @ full volume)...");
  lcd.setCursor(0,2);
  lcd.print("     ...MP3...      ");  
  lcd.setCursor(0,3);
  lcd.print(" Playing 3 seconds! ");
  mp3Player.setVolume(29);      // Sets default volume level to 0 (valid range = 0 to 30)
  Selection SDcard = {2,1,0,30,0}; // play all mp3s on SD card at full volume
  mp3Player.play(SDcard);
  myTime = millis() + 3000;
  while (millis() < myTime) {
      mp3Player.manageDevice();      // Sends requests to DFPlayer & processes responses. 
      delay(40);
  }
  mp3Player.stop();
  delay(40);
  mp3Player.manageDevice();      // Sends requests to DFPlayer & processes responses. 
  delay(40);
  mp3Player.manageDevice();      // Sends requests to DFPlayer & processes responses. 
  delay(40);
  Serial.println("DONE!");


  // === Test RFID ===
  Serial.println("=== RFID...");
  lcd.setCursor(0,2);
  lcd.print("     ...RFID...     ");
  lcd.setCursor(0,3);
  lcd.print("Scan card or dongle ");
  Serial.println("Scan card or dongle...");
  mfrc522.PCD_Init();   // Init MFRC522
  bool done = false;
  bool rfidBlink = false;
  myTimerStart = millis();
  while(!done) {
    while(!mfrc522.PICC_IsNewCardPresent()) { 
      if ((millis() - myTimerStart) > 500) {
        if (rfidBlink) {
          rfidBlink = false;
          for(int i=8; i<16; i++) {
            LedStrip.setPixelColor(i, LedStrip.Color(16, 16, 16));
          }
        } else {
          rfidBlink = true;
          for(int i=8; i<16; i++) {
            LedStrip.setPixelColor(i, LedStrip.Color(0, 0, 0));
          }
        }
        LedStrip.show();
        myTimerStart = millis();
      }
    }
    if (mfrc522.PICC_ReadCardSerial()) {
      for(int i=8; i<16; i++) {
        LedStrip.setPixelColor(i, LedStrip.Color(0, 64, 0));
      }
      LedStrip.show();
      Serial.print("Card UID: ");
      lcd.setCursor(0,3);
      lcd.print("Card ID=            ");
      lcd.setCursor(8,3);
      for (byte i=0; i < mfrc522.uid.size; i++) {
        if (i>0) {
          Serial.print(":");
          lcd.print(":");
        }
        if (mfrc522.uid.uidByte[i] < 16) {
          Serial.print("0");
          lcd.print("0");
        }
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        lcd.print(mfrc522.uid.uidByte[i], HEX);
      }
      Serial.println("...Done!");
      delay(2000);
      for(int i=8; i<16; i++) {
        LedStrip.setPixelColor(i, LedStrip.Color(0, 0, 0));
      }
      LedStrip.show();
      done = true;
    }
  }
  delay(6000);


  // === Test accelerameter ===
  Serial.println("=== Accelerometer...");
  lcd.setCursor(0,2);
  lcd.print("   ...ADXL345...    ");
  Serial.println("Initialize ADXL345");
  if (!accelerometer.begin()) {
    lcd.setCursor(0,3);
    lcd.print("      [ERROR]       ");
  } else {
    accelerometer.setTapDetectionX(1);       // Check tap on X-Axis
    accelerometer.setTapDetectionY(1);       // Check tap on Y-Axis
    accelerometer.setTapDetectionZ(1);       // Check tap on Z-Axis
    accelerometer.setTapThreshold(2.5);      // Recommended 2.5 g
    accelerometer.setTapDuration(0.02);      // Recommended 0.02 s
    accelerometer.setDoubleTapLatency(0.10); // Recommended 0.10 s
    accelerometer.setDoubleTapWindow(0.30);  // Recommended 0.30 s
    accelerometer.useInterrupt(ADXL345_INT1);
    lcd.setCursor(0,3);
    lcd.print("Do single tap...    ");
    bool done = false;
    static Activites lastActiv;
    lastActiv = accelerometer.readActivites();
    while (!done) {
      // Read values for activities
      delay(50);
      Activites activ = accelerometer.readActivites();
      if (activ.isTap & !lastActiv.isTap) {
        Serial.println("Tap Detected!");
        lcd.setCursor(0,3);
        lcd.print("Single Tap Detected!");
        done = true;
      }
      lastActiv = activ;
    }
    delay(2000);
    done = false;
    lcd.setCursor(0,3);
    lcd.print("Do double tap...    ");
    while (!done) {
      // Read values for activities
      delay(50);
      Activites activ = accelerometer.readActivites();
      if (activ.isDoubleTap & !lastActiv.isDoubleTap) {
        Serial.println("Double Tap Detected!");
        lcd.setCursor(0,3);
        lcd.print("Double Tap Detected!");
        done = true;
      }
      lastActiv = activ;
    }
    delay(1000);
  }
  delay(1000);



  // === DONE ===
  lcd.setCursor(0,2);
  lcd.print("                    ");
  lcd.setCursor(0,3);
  lcd.print("       [DONE]       ");
  delay(1000);
}




void loop() {
  // put your main code here, to run repeatedly:
}
