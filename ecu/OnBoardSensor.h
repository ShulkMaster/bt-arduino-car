#include "DHT.h"
#include "GpioMap.h"


class OnBoardSensor {
DHT dht = DHT(D5, DHT11);
unsigned long track = 0;
    public:
OnBoardSensor(){
  dht.begin();
  track = millis();
}
    

    void tick(){
      if(track + 2000 > millis()) return;
      float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  //publish temperature and humidity
  Serial.print(F("\nTemperature: "));
  Serial.print(t);
  Serial.print(F("\nHumidity: "));
  Serial.print(h);
    }
};
