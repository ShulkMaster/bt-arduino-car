#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "lib/PublishData.h"

// WIFI Stuff
#define SSID_NAME "Zuko"
#define SSID_PW "TigoZuko4840"

#define AIO_SERVER "io.adafruit.com"
#define AIO_USERNAME "ShulkMaster"
#define AIO_KEY "aio_pmhr59GnLciIwekyUwZQBTd5h911"

enum WifiManagerStatus
{
  WDisconected,
  WConnected,
  MQQTReady,
  Sending
};

class WifiManager
{
  WiFiClient client;
  WifiManagerStatus state = WDisconected;
  Adafruit_MQTT_Client *mqttConn;
  Adafruit_MQTT_Publish *lightFeed;
  Adafruit_MQTT_Publish *humidityFeed;
  Adafruit_MQTT_Publish *temperatureFeed;
  Adafruit_MQTT_Publish *tickFeed;
  unsigned long track = 0;
  int ret = -1;

public:
  WifiManager()
  {
    this->mqttConn = new Adafruit_MQTT_Client(&client, AIO_SERVER, 1883, AIO_USERNAME, AIO_KEY);
    this->lightFeed = new Adafruit_MQTT_Publish(mqttConn, AIO_USERNAME "/feeds/car-explorer.light-level");
    this->humidityFeed = new Adafruit_MQTT_Publish(mqttConn, AIO_USERNAME "/feeds/car-explorer.humidity");
    this->temperatureFeed = new Adafruit_MQTT_Publish(mqttConn, AIO_USERNAME "/feeds/car-explorer.temperature");
    this->tickFeed = new Adafruit_MQTT_Publish(mqttConn, AIO_USERNAME "/feeds/car-explorer.tick");
    this->track = millis();
  }

  void begin()
  {
    Serial.println("Connecting to WiFi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID_NAME, SSID_PW);
  }

  void tick()
  {
    switch (state)
    {
    case WDisconected:
      handleDisconnected();
      break;
    case WConnected:
      handleConneced();
      break;
    case MQQTReady:
      handleMqqt();
      break;
    case Sending:
      handleSending();
      break;
    }
  }

  void handleDisconnected()
  {
    track = millis();
    if (WiFi.status() == WL_CONNECTED)
    {
      state = WConnected;
      Serial.println();
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      ret = mqttConn->connect();
    }
  }

  void handleConneced()
  {
    if (track + 2000 > millis())
      return;
    track = millis();
    if (mqttConn->connected())
    {
      state = MQQTReady;
    }
    else
    {
      Serial.println();
      Serial.print("MQQT: ");
      Serial.println(mqttConn->connectErrorString(ret));
      state = WDisconected;
    }
  }

  void handleMqqt()
  {
    state = Sending;
    track = millis();
  }

  void handleSending()
  {
    if (track + 5000 > millis())
      return;
    track = millis();
    state = MQQTReady;
    float lightFake = millis() / 5000;
    lightFake = 50 * sin(lightFake) + 50;
    lightFeed->publish(lightFake);
  }

  void prepublish(const PublishData &data) {
    lightFeed->publish(data.lightLevel);
    humidityFeed->publish(data.humidity);
    temperatureFeed->publish(data.temperature);
    tickFeed->publish(data.tick);
  }
};
