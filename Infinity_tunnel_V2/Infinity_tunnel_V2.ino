//--- Infinity Tunnel ---//
/*********
 OTA Updates
  Random Nerd: https://RandomNerdTutorials.com/esp8266-nodemcu-ota-elegantota-arduino/

 FastLED
 Group | LED#| Description
 ------+-----+--------------------------------------------
 numA  | 0-2 | Vertical Scale | Rainbow Colours
 numB  | 3   | Choke          | Party Colours (linear blend)
 numC  | 4-5 | Light pipe     | Rainbow Stripes (no blend)
 ---------------------------------------------------------
 https://github.com/marmilicious/FastLED_examples/blob/master/palette_basic_setup2.ino
 https://github.com/marmilicious/FastLED_examples/blob/master/multiple_animations.ino

 OLED
 SSD1306 drivers, 128x64 pixel display using I2C, code from https://randomnerdtutorials.com
 Sine wave generator https://mymixblogwork.blogspot.com/2024/11/sine-wave-on-oled-display-code.html
*********/

// OTA Updates
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
//#include <Credentials.h>

AsyncWebServer server(80);

//const char* ssid = "BT-9ZACRK";
//const char* password = "nk6bMTQ4NGVRtF";
//const char* ssid = "AndroidAP";
//const char* password = "nmst3670";
//const char* host = "Mech";
//const char* ssid = "KALI 6581";
//const char* password = "uB46$616";
const char* ssid = "Galaxy A13 4403";
const char* password = "cgtb5105";


// FastLED
#include <FastLED.h>

#define LED_PIN     2
#define NUM_LEDS    6
#define BRIGHTNESS  20
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

uint8_t startIndex;
uint8_t colorIndex;

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;
// Set LED groups (numA thru numN values will add up to NUM_LEDS)
#define numA 3
#define numB 1
#define numC 2

// The array that actually gets displayed
CRGB ledsA[numA];     // ledsA thru ledsC are "working" arrays
CRGB ledsB[numB];
CRGB ledsC[numC];

// OLED
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// *** OLED damaged on top 2 1/2 rows: [display.setCursor(0, 20);] ***

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Sinewave variables
float phase = 0.0;         // Initial phase for the sine wave

float frequency = 10;       // Frequency (Hz)

int amplitude = 16;        // Amplitude of the sine wave

int centerY = 42;           // SCREEN_HEIGHT / 2; // Center of the screen

// Time
    uint32_t startTime = millis();
/*--------------------------------------------------------------------------*/
void setup() {
  Serial.begin(115200);
  Serial.println("*** Mech controlroom infinity tunnel ***");
//----------------------------------  
// FastLED
// while (millis() - startTime > 500) {      // power-up safety delay
    delay(500);
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    FastLED.clear();
    Serial.println("FastLED setup");
// }

//----------------------------------  
// OLED
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
    Serial.println(F("OLED setup"));
  // OLED Splash screen showing OTA details
   while (millis() - startTime < 4000) {
    display.clearDisplay();                     // Clear screen
    display.setTextSize(1);                     // Draw 2x-scale text
    display.setTextColor(WHITE);                // Set font colour
    display.setCursor(10, 42);                   // Cursor start point
    display.println(F("Initalising"));               // Text to print
    display.display();                          // Show initial text
//    delay(2000);                                // Wait
    display.startscrollright(0x00, 0x0F);       // Scroll right
//  delay(5000);
//  display.stopscroll();
    }
    display.clearDisplay();  

//----------------------------------    
// OTA Update
  // Set up Wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
   }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Start Server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! I am ESP8266.");
  });
  server.begin();
  Serial.println("HTTP server started");

  ElegantOTA.begin(&server);    // Start ElegantOTA
  Serial.println("OTA active");
}

void loop() {
  
// Draw OLED sinewave
  display.clearDisplay();

  // Calculate phase increment to achieve 100ms width for any frequency
  int freq = analogRead(A0);
  frequency = (float)freq * (50 - 15) / 1023 + 15;
  float phaseIncrement = (2 * PI * frequency) / (SCREEN_WIDTH * 5); // * 10 - Scales to 100ms

  // Draw the moving sine wave
  for (int x = 0; x < SCREEN_WIDTH; x++) {
    int y = centerY + int(amplitude * sin(phase + x * phaseIncrement));
    display.drawPixel(x, y, SSD1306_WHITE);
    }

  // Display the frequency at the top
  display.setTextSize(1);               // Set text size to normal
  display.setTextColor(SSD1306_WHITE);  // Set text colour to white
  display.setCursor(0, 0);              // Set cursor to the top left
  display.print("Sine wave: ");         // Print text
  display.print(frequency);             // Print the frequency value
  display.print(" Hz");                 // Print " Hz" after the frequency

  // Update the phase to animate the wave
  phase += phaseIncrement;

  // Show the wave on the display
  display.display();

  // Adjust delay based on frequency to control the wave animation speed
  //delay(10);  // Consistent animation speed for any frequency

//--- FastLED ---//

  //--------------Animation A--------------
// This function fills the palette with totally random rainbow colors.
      EVERY_N_MILLISECONDS(10) {
    colorIndex = startIndex;        
     for (uint8_t i=0; i<numA; i++) {
               //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])
      ledsA[i] = ColorFromPalette( RainbowColors_p, colorIndex, BRIGHTNESS, LINEARBLEND);
      colorIndex = colorIndex + 10;  //how fast to advance through palette
    }
  }
  
  //copy ledsA data to leds
 for (uint8_t i=0; i<numA; i++) { leds[i] = ledsA[i]; }
  

  //--------------Animation B--------------
// This function fills the palette with 'Party' colors.
      EVERY_N_MILLISECONDS(2000) {
    colorIndex = startIndex;        
     for (uint8_t i=0; i<numB; i++) {
      //  ledsB[i] = CHSV( random8(), 255, random8());
        //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])
      ledsB[i] = ColorFromPalette( PartyColors_p, colorIndex, BRIGHTNESS, LINEARBLEND);
      colorIndex = colorIndex + 300;  //how fast to advance through palette
    }
  }
  
    //copy ledsB data to leds
  for (uint8_t i=0; i<numB; i++) { leds[i+numA] = ledsB[i]; }
  
  //--------------Animation C--------------
// This function fills the palette with stripes of rainbow colors.
      EVERY_N_MILLISECONDS(10) {
    colorIndex = startIndex;        
     for (uint8_t i=0; i<numC; i++) {
      //  ledsB[i] = CHSV( random8(), 255, random8());
        //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])
      ledsC[i] = ColorFromPalette(RainbowStripeColors_p, colorIndex, BRIGHTNESS+60, LINEARBLEND);
      colorIndex = colorIndex + 10;  //how fast to advance through palette
    }
  }

  //copy ledsC data to leds
  for (uint8_t i=0; i<numC; i++) { leds[i+numA+numB] = ledsC[i]; }

  //display all the updates on leds array
  FastLED.show();
  startIndex = startIndex + 1;  //how fast to move the palette down the strip

  ElegantOTA.loop();

}
