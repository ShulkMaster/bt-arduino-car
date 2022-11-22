#include <SoftwareSerial.h>
#include "lib/SerialConnection.h"
#include "DualSerial.h"
#include "GpioMap.h"

char letters[12] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'o', 'u', 't'};
unsigned long nextTick = 0;

SerialConnection *conn = NULL;
DualSerial* dStream = new DualSerial();
bool forward = true;

void onLensChange(LensMessage);
void onStatusChange(ConnectionState);

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  conn = new SerialConnection(true, dStream);
  conn->onMessage(&onLensChange);
  conn->onMessage(&onStatusChange);
}

void loop()
{
  conn->tick();
}

short speedCalc(short distance)
{
  if (distance == 0 || distance >= 20)
    return 220;
  if (distance <= 10)
    return 0;
  return 4 * distance - 40;
}

void onStatusChange(ConnectionState s){
  String  msg;
        
  switch(s){
    case Disconected:
      msg = "Disconected";
      break;
    case Connecting:
      msg = "Connecting";
      break;
      case Connected:
      msg = "Connected";
      break;
    case Timeout:
      msg = "Timeout";
      break;
      case Invalid:
      msg = "Invalid";
      break;
      case Incomming:
      msg = "Incoming";
  }
  Serial.println(msg);
}

void onLensChange(LensMessage lnsm)
{
  SpeedMessage spm;
  const short frontDistance = lnsm.frontD;
  const short backDistance = lnsm.backD;
  short newSpeed = 0;
  newSpeed = speedCalc(forward ? frontDistance : backDistance);
  if(newSpeed < 1){
    forward = !forward;
  }

  if(!forward){
    newSpeed = -newSpeed;
  }
  
  Serial.print("Lens recevied ");
  Serial.print(frontDistance);
  Serial.print(' ');
  Serial.println(backDistance);
  Serial.print("Speeds Send ");
  Serial.println(newSpeed);
  spm.speedLeft = newSpeed;
  spm.speedRight = newSpeed;
  conn->send(spm);
}
