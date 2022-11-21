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
  conn->onMessage(&onStatusChange);
}

void onMeasure(int front, int back) {
  LensMessage msg;
  msg.frontD = front;
  msg.backD = back;
  conn->send(msg);
}

void onStatusChange(ConnectionState s){
  int led = 0;
  String msg;
  switch(s){
    case Disconected:
      msg = "Disconected";
      led = 2;
      break;
    case Connecting:
      msg = "Connecting";
      led = 3;
      break;
      case Connected:
      msg = "Connected";
      led = 4;
      break;
    case Timeout:
      msg = "Timeout";
      break;
      led = 5;
      case Invalid:
      msg = "Invalid";
      led = 6;
      break;
  }
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(led, HIGH);
  }

void loop(){
   if(conn->getState() == Connected){
    //lens->tick();
   }
   conn->tick();
}
