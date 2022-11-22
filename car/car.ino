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

bool alternator = false;
void onMeasure(short front, short back) {
  if(tracked + 50 > millis()){
    return;
    }
  LensMessage msg;
  msg.frontD = front;
  msg.backD = back;
  tracked = millis();
  conn->send(msg);
}

void onSpeedChange(SpeedMessage m){
  Serial.println();
  Serial.print(m.speedLeft);
  Serial.print(' ');
  Serial.println(m.speedRight);
  if(m.speedLeft >=0){
    transmission->advance(m.speedLeft, 90);
    return;
    }
  transmission->reverse(abs(m.speedLeft), 90);
 }

void onStatusChange(ConnectionState s){
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  switch(s){
    case Connecting:
      digitalWrite(10, HIGH);
      break;
      case Connected:
      digitalWrite(11, HIGH);
      break;
    case Timeout:
    case Invalid:
    case Disconected:
      break;
    case Incomming:
      digitalWrite(10, alternator ? HIGH : LOW);
      digitalWrite(11, alternator ? LOW : HIGH);
      alternator = !alternator;
      break;
  }
}

void loop(){
   if(conn->getState() == Connected || conn->getState() == Incomming){
    lens->tick();
   }
   conn->tick();
}
