/**
 * @file mqtt.h
 * @author Riccardo Iacob (github.com/riacob)
 * @brief
 * @version 0.1
 * @date 2025-01-29
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "config.h"

class MQTT {
 public:
  GlobalConfig *_cfg;
  bool _init;
  WiFiClient* wifiClient;
  PubSubClient* pubsubClient;
  void reconnect();
  void setup_wifi();
  bool pubSubConnect();

 public:
  MQTT(GlobalConfig *cfg);
  void init();
  void runLoop();
};

#endif /* MQTT_H */