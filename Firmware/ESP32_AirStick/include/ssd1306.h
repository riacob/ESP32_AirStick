#ifndef SSD1306_H
#define SSD1306_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class SSD1306 {
    public:
    Adafruit_SSD1306 *_ssd1306;
    TwoWire *_wire;
    public:
    SSD1306(TwoWire *twi);
    void init();
};

#endif