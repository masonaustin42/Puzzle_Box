/// @file    FirstLight.ino
/// @brief   Animate a white dot moving along a strip of LEDs
/// @example FirstLight.ino

// Use if you want to force the software SPI subsystem to be used for some reason (generally, you don't)
// #define FASTLED_FORCE_SOFTWARE_SPI
// Use if you want to force non-accelerated pin access (hint: you really don't, it breaks lots of things)
// #define FASTLED_FORCE_SOFTWARE_SPI
// #define FASTLED_FORCE_SOFTWARE_PINS
#include <WS2812Serial.h>
#define USE_WS2812SERIAL
#include <FastLED.h>

///////////////////////////////////////////////////////////////////////////////////////////
//
// Move a white dot along the strip of leds.  This program simply shows how to configure the leds,
// and then how to turn a single pixel white and then off, moving down the line of pixels.
// 

// How many leds are in the strip?
#define NUM_LEDS 93

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 1

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];


uint8_t hue = 0;

const int Rings[][2] = {{0, 31},
                          {32, 55},
                          {56, 71},
                          {72, 83},
                          {84, 91}};

// This function sets up the ledsand tells the controller about them
void setup() {
	// sanity check delay - allows reprogramming if accidently blowing power w/leds
   	delay(2000);

    FastLED.addLeds<WS2812SERIAL, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
    FastLED.setBrightness(10);
    FastLED.setDither(0);
}

// This function runs over and over, and is where you do the magic to light
// your leds.
void loop() {
   
  // sweep(true);
  swirl_from_middle();
  // pulses();

  EVERY_N_MILLISECONDS(10) {
    hue++;
  }

  FastLED.show();
}

void sweep(bool spiral) {

  uint8_t ring1 = map(beat8(30,600 * spiral), 0, 255, Rings[0][0], Rings[0][1]);
  uint8_t ring2 = map(beat8(30,450 * spiral), 0, 255, Rings[1][0], Rings[1][1]);
  uint8_t ring3 = map(beat8(30,300 * spiral), 0, 255, Rings[2][0], Rings[2][1]);
  uint8_t ring4 = map(beat8(30,150 * spiral), 0, 255, Rings[3][0], Rings[3][1]);
  uint8_t ring5 = map(beat8(30,0), 0, 255, Rings[4][0], Rings[4][1]);

  leds[ring1] = CHSV(hue, 200, 255);
  leds[ring2] = CHSV(hue, 200, 255);
  leds[ring3] = CHSV(hue, 200, 255);
  leds[ring4] = CHSV(hue, 200, 255);
  leds[ring5] = CHSV(hue, 200, 255);
  leds[92] = CHSV(hue, 200, 255);

  fadeToBlackBy(leds, NUM_LEDS, 3);
}

void swirl_from_middle() {
  uint8_t ringSelect = beatsin8(20, 0, 4, 0, 180);
  uint8_t saw = beat8(65, 0);
  int angle = map(saw, 0, 255, 0, 360);
  int angle2 = angle + 120;
  int angle3 = angle + 240;
  if (angle2 > 360) angle2 -= 360;
  if (angle3 > 360) angle3 -= 360;
  uint8_t pos1 = map(angle, 0, 360, Rings[ringSelect][0], Rings[ringSelect][1]);
  uint8_t pos2 = map(angle2, 0, 360, Rings[ringSelect][0], Rings[ringSelect][1]);
  uint8_t pos3 = map(angle3, 0, 360, Rings[ringSelect][0], Rings[ringSelect][1]);

  
  leds[pos1] = CHSV(hue, 200, 255);
  leds[pos2] = CHSV(hue + 85, 200, 255);
  leds[pos3] = CHSV(hue + 170, 200, 255);
  leds[92] = CHSV(hue, 200, 255);

   fadeToBlackBy(leds, NUM_LEDS, 3);
}

void pulses() {
  uint8_t tempHue = hue;

  uint8_t i = map(beat8(60, 0), 0, 255, 20, 0);
  if (i == 5) {
    leds[92] = CHSV(tempHue, 200, 255);
  } else if (i < 5) {
    for (int j = Rings[i][0]; j <= Rings[i][1]; j++) {
      leds[j] = CHSV(tempHue, 200, 255);
    }
  }
  fadeToBlackBy(leds, NUM_LEDS, 4);
  
}
