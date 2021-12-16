// Simple program for LED-lit modernized moroccan lamp project
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


#define BRIGHTNESS_BTN_PIN  1

#define COLOR_BTN_PIN       2

#define PIXEL_PIN           0  // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT         1  // Number of NeoPixels


Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

boolean oldColorState = HIGH;
boolean oldBrightnessState = HIGH;
int     mode     = 0;    // Currently-active mode, 0-9



void setup() {
  pinMode(COLOR_BTN_PIN, INPUT_PULLUP);
  pinMode(BRIGHTNESS_BTN_PIN, INPUT_PULLUP);
  
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
}

void loop() {
  watchColorButton();
  watchBrightnessButton();
}

void watchBrightnessButton () {
  boolean newBrightnessState = digitalRead(BRIGHTNESS_BTN_PIN);

  // Check if button was pressed
  if((newBrightnessState == LOW) && (oldBrightnessState == HIGH)){
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.8
    newBrightnessState = digitalRead(BRIGHTNESS_BTN_PIN);
    if(newBrightnessState == LOW) {      // Yes, still low
      if(++mode > 8) mode = 0; // Advance to next mode, wrap around after #4
      switch(mode) {           // Start the new animation...
        case 0:
          setBrightness(0);    // Black/off
          break;
        case 1:
          setBrightness(30); // aiming for amber
          break;
        case 2:
          setBrightness(80); // aiming for amber
          break;
        case 3:
          setBrightness(255); // aiming for amber
          break;
        case 4:
          setBrightness(190); // Red
          break;
        case 5:
          setBrightness(190); // Green
          break;
        case 6:
          setBrightness(190); // Blue
          break;
        case 7:
          setBrightness(50); // White
          break;
        case 8:
          setBrightness(255); // White
          break;
      }
    }
  }

  oldBrightnessState = newBrightnessState;
}

void watchColorButton () {
    boolean newColorState = digitalRead(COLOR_BTN_PIN);

  // Check if button was pressed
  if((newColorState == LOW) && (oldColorState == HIGH)){
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newColorState = digitalRead(COLOR_BTN_PIN);
    if(newColorState == LOW) {      // Yes, still low
      if(++mode > 8) mode = 0; // Advance to next mode, wrap around after #8
      switch(mode) {           // Start the new animation...
        case 0:
          setColor(strip.Color(0, 0, 0));    // Black/off
          break;
        case 1:
          setColor(strip.Color(255,50,0,255)); // aiming for amber
          break;
        case 2:
          setColor(strip.Color(255,50,0)); // aiming for amber
          break;
        case 3:
          setColor(strip.Color(255,50,255));
          break;
        case 4:
          setColor(strip.Color(255,0,255));
          break;
        case 5:
          setColor(strip.Color(0,255,0)); // Green
          break;
        case 6:
          setColor(strip.Color(0,0,255)); // Blue
          break;
        case 7:
          setColor(strip.Color(255,0,0)); // Red
          break;
        case 8:
          setColor(strip.Color(255,255,255)); // White
          break;
      }
    }
  }

  oldColorState = newColorState;
}

void setColor(uint32_t color) {
  strip.setPixelColor(0, color);
  strip.show();
}

void setBrightness(uint8_t brightness) {
  strip.setBrightness(brightness); //(max = 255)
  strip.show();
}

void setColorAndBrightness(uint32_t color, int brightness){
  strip.setPixelColor(0, color);
  strip.setBrightness(brightness); //(max = 255)
  strip.show();
}

// The below seem to block use of the button...

// void colorCycle(int wait, uint8_t brightness) {
//  for(long color = 0; color < 5*65536; color += 256) {
//    strip.rainbow(color, 9999, 255, brightness, true);
//    strip.show();
//    delay(wait);
//  }  
// }

//// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
//void rainbow(int wait) {
//  // Hue of first pixel runs 5 complete loops through the color wheel.
//  // Color wheel has a range of 65536 but it's OK if we roll over, so
//  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
//  // means we'll make 5*65536/256 = 1280 passes through this loop:
//  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
//    // strip.rainbow() can take a single argument (first pixel hue) or
//    // optionally a few extras: number of rainbow repetitions (default 1),
//    // saturation and value (brightness) (both 0-255, similar to the
//    // ColorHSV() function, default 255), and a true/false flag for whether
//    // to apply gamma correction to provide 'truer' colors (default true).
//    strip.rainbow(firstPixelHue);
//    // Above line is equivalent to:
//    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
//    strip.show(); // Update strip with new contents
//    delay(wait);  // Pause for a moment
//  }
//}