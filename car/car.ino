#define LDRPin A0

#include "Transmission.h"
#include "Lens.h"
#include "lib/SerialConnection.h"


Transmission *transmission = NULL;
Lens *lens = NULL;
SerialConnection *conn = NULL;
unsigned long tracked = 0;
bool forward = true;
bool stopped = false;
SpeedMessage speedy;
int statusLdr;
int ticks = 0;

void setup()
{
  Serial.begin(115200);
  Engine *en1 = new Engine(3, 2, 4);
  Engine *en2 = new Engine(5, 6, 7);
  transmission = new Transmission(en1, en2);
  lens = new Lens(9, 13, 8, 12);
  lens->registerCallback(&onMeasure);
  conn = new SerialConnection(false, &Serial);
  conn->onMessage(&onStatusChange);
  conn->onMessage(&onContinueChange);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(LDRPin, INPUT);
  digitalWrite(10, HIGH);
  digitalWrite(11, LOW);
  speedy.speedLeft = 0;
  speedy.speedRight = 0;
}

bool alternator = false;

short speedCalc(short distance)
{
  if (distance == 0 || distance >= 10) {
    return 160;
  }
  return 0;
}

void onMeasure(short front, short back)
{
  short newSpeed = 0;
  newSpeed = speedCalc(forward ? front : back);
  if (newSpeed < 1)
  {
    forward = !forward;
  }

  if (!forward)
  {
    newSpeed = -newSpeed;
    ticks--;
  }else{
    ticks++;
    }
  speedy.speedLeft = newSpeed;
  speedy.speedRight = newSpeed;
  SensorMessage msg;
  msg.tick = ticks;
  msg.lightLevel = statusLdr;
  transmission->stop();
  conn->send(msg);
  Serial.print('W');
  if(stopped) return;
  transmission->move(speedy);
}

void onContinueChange(ContinueMessage msg){
  stopped = msg.shouldContinue;
  if(msg.shouldContinue){
    transmission->move(speedy);
  }else {
    transmission->stop();
  }
}

void onStatusChange(ConnectionState s)
{
  switch (s)
  {
  case Connecting:
  case Timeout:
  case Invalid:
  case Disconected:
    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);
    break;
  case Connected:
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    break;
  case Incomming:
    digitalWrite(10, LOW);
    digitalWrite(11, alternator ? HIGH : LOW);
    alternator = !alternator;
    break;
  }
}

void loop()
{
  if (conn->available())
  {
    lens->tick();
  }
  statusLdr = analogRead(LDRPin);
  conn->tick();
  if (millis() - conn->lastReceived() > ConnectionWaitTimeout && tracked + 500 < millis())
  {
    transmission->stop();
    Serial.print('.');
    tracked = millis();
  }
}
