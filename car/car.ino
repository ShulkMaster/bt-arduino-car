#include "Transmission.h"
#include "Lens.h"
#include "lib/SerialConnection.h"

Transmission* transmission = NULL;
Lens* lens = NULL;
SerialConnection* conn = NULL;
unsigned long tracked = 0;

void setup(){
  Serial.begin(115200);
  Engine* en1 = new Engine(3,2,4);
  Engine* en2 = new Engine(5,6,7);
  transmission = new Transmission(en1, en2);
  lens = new Lens(9, 13, 8, 12);
  lens->registerCallback(&onMeasure);
  conn = new SerialConnection(false, &Serial);
  conn->onMessage(&onStatusChange);
  conn->onMessage(&onSpeedChange);
}

void onMeasure(short front, short back) {
  LensMessage msg;
  msg.frontD = front;
  msg.backD = back;
  conn->send(msg);
}

void onSpeedChange(SpeedMessage m){
  //Serial.print(m.speeedLeft);
 }

void onStatusChange(ConnectionState s){
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  switch(s){
    case Disconected:
      digitalWrite(2, HIGH);
      break;
    case Connecting:
      digitalWrite(4, HIGH);
      break;
      case Connected:
      digitalWrite(3, HIGH);
      break;
    case Timeout:
      case Invalid:
      break;
    case Incomming:
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
     break;
  }
  }

void loop(){
   if(conn->getState() == Connected || conn->getState() == Incomming){
    if(tracked + 500 < millis()){
      int x = 110;
      float rads = (PI * 2)/100;
      x += 12 * sin(rads * millis());
      onMeasure(1069, 496);
      tracked = millis();
     }
    //lens->tick();
   }
   conn->tick();
}
