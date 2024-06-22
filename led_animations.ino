void ledPulsing() {
  // uint8_t tempHue = hue;

  uint8_t i = map(beat8(60, 0), 0, 255, 20, 0);
  if (i == 5) {
    leds[92] = CHSV(PointHues[objective], 200, 255);
  } else if (i < 5) {
    for (int j = Rings[i][0]; j <= Rings[i][1]; j++) {
      leds[j] = CHSV(PointHues[objective], 200, 255);
    }
  }
  fadeToBlackBy(leds, NUM_LEDS, 4);
}

void ledSearching(bool spiral) {
  uint8_t saturation = beat8(30, 0);
  uint8_t ring1 = map(beat8(30,600 * spiral), 0, 255, Rings[0][0], Rings[0][1]);
  uint8_t ring2 = map(beat8(30,450 * spiral), 0, 255, Rings[1][0], Rings[1][1]);
  uint8_t ring3 = map(beat8(30,300 * spiral), 0, 255, Rings[2][0], Rings[2][1]);
  uint8_t ring4 = map(beat8(30,150 * spiral), 0, 255, Rings[3][0], Rings[3][1]);
  uint8_t ring5 = map(beat8(30,0), 0, 255, Rings[4][0], Rings[4][1]);

  leds[ring1] = CHSV(85, saturation, 255);
  leds[ring2] = CHSV(85, saturation, 255);
  leds[ring3] = CHSV(85, saturation, 255);
  leds[ring4] = CHSV(85, saturation, 255);
  leds[ring5] = CHSV(85, saturation, 255);
  leds[92] = CHSV(85, saturation, 255);

  // leds[ring1] = CRGB::Red;
  // leds[ring2] = CRGB::Red;
  // leds[ring3] = CRGB::Red;
  // leds[ring4] = CRGB::Red;
  // leds[ring5] = CRGB::Red;
  // leds[92] = CRGB::Red;

  fadeToBlackBy(leds, NUM_LEDS, 3);
}

void ledPoints() {
  FastLED.clear();

  int selectedRing = 0;
  if (distance > 0.1 && distance <= 1) {
    selectedRing = 4;
  } else if (distance > 1 && distance <= 2) {
    selectedRing = 3;
  } else if (distance > 2 && distance <= 3) {
    selectedRing = 2;
  } else if (distance > 3 && distance <= 4) {
    selectedRing = 1;
  } else {
    selectedRing = 0;
  }

    int offset = 0;
    for (int i = 0; i < selectedRing; i++) {
      offset += RingSizes[i];
    }

    int pointLed = ((bearing / 360) * RingSizes[selectedRing]) + offset;
    leds[pointLed].setHue(PointHues[objective]);
}
