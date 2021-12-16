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

int     mode     = 0;    // Currently-active mode, 0-9
boolean oldColorState      = HIGH;
boolean oldBrightnessState = HIGH;
boolean oldComboState [2] = { HIGH, HIGH };

uint8_t brightness         = 25; // Invisible below 25 (max = 255)
uint16_t hue               = 0;


void setup() {
  pinMode(COLOR_BTN_PIN, INPUT_PULLUP);
  pinMode(BRIGHTNESS_BTN_PIN, INPUT_PULLUP);
  
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  setHue();            // Turn OFF all pixels ASAP
}

void loop() {
  watchColorButton();
  watchBrightnessButton();
  watchComboPress();
}

void watchComboPress () {
  boolean newComboState [2] = { digitalRead(BRIGHTNESS_BTN_PIN), digitalRead(COLOR_BTN_PIN)};

  if((newComboState == { LOW, LOW }) && (oldComboState == { HIGH, HIGH })) {
    delay(20);

    newComboState = { digitalRead(BRIGHTNESS_BTN_PIN), digitalRead(COLOR_BTN_PIN)};

    if(newComboState == { LOW, LOW}) {
      if(strip.getBrightness() == 0) {
        strip.setBrightness(25);
      } else {
        strip.setBrightness(0);
      }
        strip.show();
    }

  oldComboState = newComboState;
  }
}

void watchBrightnessButton () {
  boolean newBrightnessState = digitalRead(BRIGHTNESS_BTN_PIN);

  // Check if button was pressed
  if((newBrightnessState == LOW) && (oldBrightnessState == HIGH)) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.8
    newBrightnessState = digitalRead(BRIGHTNESS_BTN_PIN);
    if (newBrightnessState == LOW) {      // Yes, still low
      if (brightness < 255) {
        brightness += 1;
      } else {
        brightness = 25;
      }

      setHue();
    }
  }
}

void setBrightness() {
  strip.setBrightness(brightness); 
  strip.show();
}

void watchColorButton () {
  boolean newColorState = digitalRead(COLOR_BTN_PIN);

  // Check if button was pressed
  if((newColorState == LOW) && (oldColorState == HIGH)) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newColorState = digitalRead(COLOR_BTN_PIN);
    if(newColorState == LOW) {      // Yes, still low
    // Color wheel has a range of 65536 but it's OK if we roll over
      if(hue < 65536) {
        hue += 256;
      } else {
        hue = 0;
      }
      
      setHue();
    }
  }
}

void setHue () {
   strip.rainbow(hue, 1, 255, brightness, true);
   strip.show();
}

void setColor(uint32_t color) {
  strip.setPixelColor(0, color);
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