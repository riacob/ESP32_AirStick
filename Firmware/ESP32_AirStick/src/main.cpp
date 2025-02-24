/**
 * @file main.cpp
 * @author Riccardo Iacob (github.com/riacob)
 * @brief ThermoHygroStick entry point and related functions
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <Arduino.h>
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <Wire.h>
#include <esp_log.h>

#include "Fonts/FreeMono12pt7b.h"
#include "aht20.h"
#include "config.h"
#include "mqtt.h"
#include "mqtt_cfg.h"
#include "ssd1306.h"


#define PIN_SDA     20
#define PIN_SCL     21
#define PIN_PWR     5
#define SERIAL_BAUD 115200

GlobalConfig cfg;
AHT20 aht(&Wire);
MQTT mqtt(&cfg);
SSD1306 oled(&Wire);

String TOPIC_DISCOVERY_TEMPERATURE, PAYLOAD_DISCOVERY_TEMPERATURE, TOPIC_DISCOVERY_HUMIDITY, PAYLOAD_DISCOVERY_HUMIDITY, TOPIC_TEMPERATURE, TOPIC_HUMIDITY;

String genDiscoveryTopicTemperature();
String genDiscoveryTopicHumidity();
String genTopicTemperature();
String genTopicHumidity();
String genDiscoveryPayloadTemperature();
String genDiscoveryPayloadHumidity();

void setup() {
  pinMode(PIN_PWR, OUTPUT);
  digitalWrite(PIN_PWR, HIGH);
  Serial.println("pwr pin init");

  Serial.begin(SERIAL_BAUD);
  Serial.println("serial init");
  Wire.begin(PIN_SDA, PIN_SCL, 100000);
  Serial.println("i2c init");
  aht.init();
  Serial.println("aht20 init");
  oled.init();
  Serial.println("oled init");
  mqtt.init();
  Serial.println("mqtt init");

  TOPIC_DISCOVERY_TEMPERATURE = genDiscoveryTopicTemperature();
  PAYLOAD_DISCOVERY_TEMPERATURE = genDiscoveryPayloadTemperature();
  TOPIC_DISCOVERY_HUMIDITY = genDiscoveryPayloadHumidity();
  PAYLOAD_DISCOVERY_HUMIDITY = genDiscoveryPayloadHumidity();
  TOPIC_TEMPERATURE = genTopicTemperature();
  TOPIC_HUMIDITY = genTopicHumidity();
  Serial.println("mqtt params init");

  ///* Publish MQTT discovery */
  //mqtt.runLoop();
  //// Discovery for temperature sensor
  //mqtt.pubsubClient->publish(TOPIC_DISCOVERY_TEMPERATURE.c_str(), PAYLOAD_DISCOVERY_TEMPERATURE.c_str());
  //// Discovery for humidity sensor
  //mqtt.pubsubClient->publish(TOPIC_DISCOVERY_HUMIDITY.c_str(), PAYLOAD_DISCOVERY_HUMIDITY.c_str());
  //Serial.println("mqtt discovery init");
}

void loop() {
  /* Read sensor */
  //mqtt.runLoop();
  if (!aht.read()) {
    /* Publish readings to MQTT */
    String temp = String(aht.getCelsius());
    String rh = String(aht.getHumidity());
    oled._ssd1306->clearDisplay();
    //oled._ssd1306->setFont(&FreeMono12pt7b);
    oled._ssd1306->setCursor(4, 4);
    oled._ssd1306->print(temp);
    oled._ssd1306->println(" C");
    oled._ssd1306->print(rh);
    oled._ssd1306->println(" %");
    oled._ssd1306->display();
    //mqtt.pubsubClient->publish(TOPIC_TEMPERATURE.c_str(), temp.c_str());
    //mqtt.pubsubClient->publish(TOPIC_HUMIDITY.c_str(), rh.c_str());
    //Serial.println("Data published to MQTT");
  }
  /* Wait 5 seconds */
  delay(5000);
}

String genDiscoveryTopicTemperature() {
  String clientId = "ESP32-THS-";
  clientId += String(WiFi.macAddress());
  return String("homeassistant/sensor/" + clientId + "_temperature/config");
}

String genDiscoveryTopicHumidity() {
  String clientId = "ESP32-THS-";
  clientId += String(WiFi.macAddress());
  return String("homeassistant/sensor/" + clientId + "_humidity/config");
}

String genDiscoveryPayloadTemperature() {
  String clientId = "ESP32-THS-";
  clientId += String(WiFi.macAddress());
  return String("{\"name\":\"ESP32 Temperature\",\"state_topic\":\"homeassistant/sensor/" + clientId + "_temperature/state\",\"unit_of_measurement\":\"°C\",\"device_class\": \"temperature\"}");
}

String genDiscoveryPayloadHumidity() {
  String clientId = "ESP32-THS-";
  clientId += String(WiFi.macAddress());
  return String("{\"name\":\"ESP32 Humidity\",\"state_topic\":\"homeassistant/sensor/" + clientId + "_humidity/state\",\"unit_of_measurement\":\"%\",\"device_class\": \"humidity\"}");
}

String genTopicTemperature() {
  String clientId = "ESP32-THS-";
  clientId += String(WiFi.macAddress());
  return String("homeassistant/sensor/" + clientId + "_temperature/state");
}

String genTopicHumidity() {
  String clientId = "ESP32-THS-";
  clientId += String(WiFi.macAddress());
  return String("homeassistant/sensor/" + clientId + "_humidity/state");
}