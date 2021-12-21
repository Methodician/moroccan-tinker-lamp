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

boolean isOn = true;
boolean shouldColorCycle   = false;
boolean oldColorState      = HIGH;
boolean oldBrightnessState = HIGH;
boolean oldComboState [2]  = { HIGH, HIGH };

uint8_t brightness         = 25; // Invisible below 25 (max = 255)
uint16_t hue               = 0;


void setup() {
  pinMode(COLOR_BTN_PIN, INPUT_PULLUP);
  pinMode(BRIGHTNESS_BTN_PIN, INPUT_PULLUP);
  
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  setHue();
}

void loop() {

  watchColorButton();
  watchBrightnessButton();
  watchComboPress();
  colorCycle();
  
}

void watchBrightnessButton () {
  boolean newBrightnessState = digitalRead(BRIGHTNESS_BTN_PIN);

  // Check if button was pressed
  if((newBrightnessState == LOW) && (oldBrightnessState == HIGH)) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newBrightnessState = digitalRead(BRIGHTNESS_BTN_PIN);
    if (newBrightnessState == LOW) {      // Yes, still low
      if (brightness < 254) {
        brightness += 1;
      } else {
        brightness = 25;
      }

      setHue();
    }
  }
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

void watchComboPress () {
  boolean newComboState [2] = { digitalRead(BRIGHTNESS_BTN_PIN), digitalRead(COLOR_BTN_PIN) };

  if(
      (newComboState[0] == LOW) &&
      (newComboState[1] == LOW) &&
      (oldComboState[0] == HIGH) &&
      (oldComboState[1] == HIGH )
    ) {
    
    delay(60);

    newComboState[0] = digitalRead(BRIGHTNESS_BTN_PIN);
    newComboState[1] = digitalRead(COLOR_BTN_PIN);

    if(newComboState[0] == LOW && newComboState[1] == LOW) {
        if(shouldColorCycle) {
            shouldColorCycle = false;
        } else {
            shouldColorCycle = true;
        }
    }

  }
  oldComboState[0] = newComboState[0];
  oldComboState[1] = newComboState[1];
}

void setHue () {
  if(!isOn) {
    brightness = 0;
  } else {
    if(brightness == 0) {
      brightness = 25;
      hue = 0;
    }
  }
   strip.rainbow(hue, 1, 255, brightness, true);
   strip.show();
}


// The below seem to block use of the button...
unsigned long lastCycleMillis = 0;
void colorCycle() {
    int delay = 30;
    unsigned long timeSinceLastCycle = millis() - lastCycleMillis;
    
    if(shouldColorCycle && timeSinceLastCycle > delay) {
        if(hue < 65536) {
            hue += 256;
        } else {
            hue = 0;
        }
        setHue();
        lastCycleMillis = millis();
    }


//  for(long color = 0; color < 5*65536; color += 256) {
//    strip.rainbow(color, 1, 255, brightness, true);
//    strip.show();
//    delay(wait);
//  }  
}