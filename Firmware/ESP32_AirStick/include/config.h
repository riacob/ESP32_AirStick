#ifndef _CONFIG_H
#define _CONFIG_H

#include <Arduino.h>
#include "cfg_pvt.h"

class GlobalConfig {
 public:
  String wifi_ssid = WIFI_SSID;
  String wifi_password = WIFI_PSK;
  String mqtt_server = "192.168.1.112";
  uint32_t mqtt_port = 1883;
  bool init = true;
  String mqtt_user = "";
  String mqtt_password = "";
  bool mqtt_auth = false;
  uint8_t ip_address[4];
  uint8_t subnet_mask[4];
  uint8_t dns[2][4];
  bool dhcp = true;
};

#endif