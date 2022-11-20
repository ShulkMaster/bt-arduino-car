#include "Transmission.h"
#include "Lens.h"
#include "lib/SerialConnection.h"

Transmission* transmission = NULL;
Lens* lens = NULL;
SerialConnection* conn = NULL;

void setup(){
  Serial.begin(115200);
  Engine* en1 = new Engine(3,2,4);
  Engine* en2 = new Engine(5,6,7);
  transmission = new Transmission(en1, en2);
  lens = new Lens(9, 13, 8, 12);
  lens->registerCallback(&onMeasure);
  conn = new SerialConnection(false, &Serial);
}

void onMeasure(int front, int back) {
  LensMessage msg;
  msg.frontD = front;
  msg.backD = back;
  conn->send(msg);
}

void loop(){
   lens->tick();
   conn->tick();
}
