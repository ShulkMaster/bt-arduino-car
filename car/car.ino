#include "Transmission.h"
#include "Lens.h"

Transmission* transmission = NULL;
Lens* lens = NULL;
unsigned long track = 0;

void setup(){
  Serial.begin(115200);
  Engine* en1 = new Engine(3,2,4);
  Engine* en2 = new Engine(5,6,7);
  transmission = new Transmission(en1, en2);
  lens = new Lens(9, 13, 8, 12);
  lens->registerCallback(&onMeasure);
}

void onMeasure(int front, int back) {
}

void loop(){
   lens->tick();
   if (Serial.available() >= 4) {
    int received = 0;
    byte intReceived[4] = {0};
    Serial.readBytes(intReceived, 4);
    for(int x = 0; x < 4; x++){
      received = (received << 8) + intReceived[x];
      Serial.println(intReceived[x]);
    }
    Serial.print("Received ");
    Serial.println(received);
   }else {
    if(track + 1000 < millis()) {
      Serial.println("timeout ");
      track = millis();
     }
   }
}
