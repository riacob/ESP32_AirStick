#include "mqtt.h"

void MQTT::setup_wifi()
{
  if (_cfg)
  {
    if (!_cfg->init)
    {
      Serial.println("ERROR: global configuration not initialized");
      return;
    }
  }
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(_cfg->wifi_ssid);
  Serial.print("...");
  /* TODO static networking config */
  WiFi.begin(_cfg->wifi_ssid, _cfg->wifi_password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi!");
}

bool MQTT::pubSubConnect()
{
  // Create a mac-based client id
  String clientId = "ESP32-THS-";
  clientId += String(WiFi.macAddress());
  /* Authenticated connection */
  if (_cfg->mqtt_auth)
  {
    return pubsubClient->connect(clientId.c_str(), _cfg->mqtt_user.c_str(), _cfg->mqtt_password.c_str());
  }
  /* Open connection */
  else
  {
    return pubsubClient->connect(clientId.c_str());
  }
}

void MQTT::reconnect()
{
  // Loop until we're reconnected
  while (!pubsubClient->connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (pubSubConnect())
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(pubsubClient->state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

MQTT::MQTT(GlobalConfig *cfg)
{
  wifiClient = new WiFiClient();
  pubsubClient = new PubSubClient(*wifiClient);
  _cfg = cfg;
}

void MQTT::init()
{
  setup_wifi();
  if (_cfg)
  {
    if (!_cfg->init)
    {
      Serial.println("ERROR: global configuration not initialized");
      return;
    }
  }
  pubsubClient->setServer(_cfg->mqtt_server.c_str(), _cfg->mqtt_port);
}

void MQTT::runLoop()
{
  if (!pubsubClient->connected())
  {
    reconnect();
  }
  pubsubClient->loop();
}