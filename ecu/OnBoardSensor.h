#include "DHT.h"
#include "GpioMap.h"


class OnBoardSensor {
DHT dht = DHT(D5, DHT11);
 public:
float h;
float t;

OnBoardSensor(){
  dht.begin();
}
    
    void tick(){
      h = dht.readHumidity();
      t = dht.readTemperature();
    }
};
