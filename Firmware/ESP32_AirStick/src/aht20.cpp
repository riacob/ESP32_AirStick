/**
 * @file aht20.cpp
 * @author Riccardo Iacob (github.com/riacob)
 * @brief Implementation file for aht20.h library
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "aht20.h"

#define AHT_ADD              0x38       /* 7-bit address */
#define AHT_CMD_INIT         0b10111110 /* Initialization command */
#define AHT_CMD_INIT_PAR0    0x08       /* Initialization command first parameter */
#define AHT_CMD_INIT_PAR1    0x00       /* Initialization command second parameter */
#define AHT_CMD_TRIGGER      0xAC       /* Trigger measurement command */
#define AHT_CMD_TRIGGER_PAR0 0x33       /* Trigger command first parameter */
#define AHT_CMD_TRIGGER_PAR1 0x00       /* Trigger command second parameter */
#define AHT_CMD_RST          0b10111010 /* Soft reset command */
#define AHT_MASK_BUSY        0b00000001 /* Mask for busy flag in status byte */
#define AHT_MASK_CAL         0b00010000 /* Mask for incal flag in status byte */
#define AHT_MASK_RH          0xFFFFF000 /* Mask for clearing extra humidity bits */
#define AHT_MASK_TEMP        0x000FFFFF /* Mask for clearing extra temperature bits */

AHT20::AHT20(TwoWire *twi) {
  _wire = twi;
}

void AHT20::init() {
  uint8_t status;
  /* Read status byte */
  _wire->requestFrom(AHT_ADD, 1);
  while (_wire->available() < 1) {
    delay(5);
  }
  status = _wire->read();
  /* Check whether calbibration bit is set */
  if (!(status & AHT_MASK_CAL)) {
    Serial.println("[Init] Calibration bit not set, calibrating...");
    /* If not, we need to initialize calibration */
    _wire->beginTransmission(AHT_ADD);
    _wire->write(AHT_CMD_INIT);
    _wire->write(AHT_CMD_INIT_PAR0);
    _wire->write(AHT_CMD_INIT_PAR1);
    _wire->endTransmission();
    Serial.println("[Init] Calibration complete!");
    /* Wait for 10mS after calibration */
    delay(10);
  }
  Serial.println("[Init] Calibration bit is set");
}

uint8_t AHT20::read() {
  uint8_t buf[7];
  uint32_t raw_temp, raw_rh;
  /* Trigger measurement */
  _wire->beginTransmission(AHT_ADD);
  _wire->write(AHT_CMD_TRIGGER);
  _wire->write(AHT_CMD_TRIGGER_PAR0);
  _wire->write(AHT_CMD_TRIGGER_PAR1);
  _wire->endTransmission();
  /* Wait for 80mS after triggering measurement */
  delay(80);
  /* Read data */
  _wire->requestFrom(AHT_ADD, 7);
  while (_wire->available() < 7) {
    Serial.print("[Read] Data bytes available: ");
    Serial.println(_wire->available());
    delay(5);
  }
  for (int i = 0; i < 7; i++) {
    buf[i] = _wire->read();
  }
  /* If the busy flag is set, the measurement is invalid */
  if (!(buf[1] & AHT_MASK_BUSY)) {
    Serial.println("[Read] ERROR: DEVICE BUSY");
    return AHT_ERR_BUSY;
  }
  /* Set last read millis */
  _lastReadMillis = millis();
  /* Parse humidity (20 bit) */
  raw_rh = 0;
  raw_rh = buf[1];
  raw_rh = (raw_rh << 8);
  raw_rh |= buf[2];
  raw_rh = (raw_rh << 8);
  raw_rh |= buf[3];
  raw_rh &= AHT_MASK_RH;
  raw_rh = (raw_rh >> 4);
  _rh = (((float)raw_rh / 1048576.0) * 100.0);  // 1048576 = 2^20 (the raw range)
  /* Parse temperature (20-bit) */
  raw_temp = 0;
  raw_temp = buf[3];
  raw_temp = (raw_temp << 8);
  raw_temp |= buf[4];
  raw_temp = (raw_temp << 8);
  raw_temp |= buf[5];
  raw_temp &= AHT_MASK_TEMP;
  _temp = ((((float)raw_temp / 1048576.0) * 200.0) - 50.0);  // 1048576 = 2^20 (the raw range)
  /* Log */
  Serial.print("[Read] Temperature: ");
  Serial.print(_temp);
  Serial.print("C, Humidity: ");
  Serial.print(_rh);
  Serial.println("%");
  return AHT_ERR_NONE;
}

float AHT20::getCelsius() {
  return _temp;
}

float AHT20::getHumidity() {
  return _rh;
}