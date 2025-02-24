/**
 * @file aht20.h
 * @author Riccardo Iacob (github.com/riacob)
 * @brief AHT20 temperature/humidity sensor interface library
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef AHT20_H
#define AHT20_H

#include <Arduino.h>
#include <Wire.h>

class AHT20 {
 private:
  TwoWire *_wire;
  float _temp;
  float _rh;
  uint32_t _lastReadMillis;
 public:
    enum readError {
    AHT_ERR_NONE = 0,
    AHT_ERR_BUSY = 1
  };
  AHT20(TwoWire *twi);
  void init();
  uint8_t read();
  float getCelsius();
  float getHumidity();
};

#endif /* AHT20_H */