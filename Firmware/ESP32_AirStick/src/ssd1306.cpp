#include "ssd1306.h"

#define SCREEN_W 128
#define SCREEN_H 64
#define SSD_ADD  0x3C

SSD1306::SSD1306(TwoWire *twi) {
  _wire = twi;
  _ssd1306 = new Adafruit_SSD1306(SCREEN_W, SCREEN_H, _wire, -1);
}

void SSD1306::init() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!_ssd1306->begin(SSD1306_SWITCHCAPVCC, SSD_ADD)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  // Don't proceed, loop forever
  }
  _ssd1306->display();
}

