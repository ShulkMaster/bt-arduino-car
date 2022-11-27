#include <SoftwareSerial.h>
#include "lib/SerialConnection.h"
#include "DualSerial.h"
#include "WifiManage.h"
#include "GpioMap.h"

unsigned long nextTick = 0;
bool measuring = false;
WifiManager manager;
SerialConnection *conn = NULL;
DualSerial *dStream = new DualSerial();

void onStatusChange(ConnectionState);

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  conn = new SerialConnection(true, dStream);
  conn->onMessage(&onStatusChange);
  nextTick = millis();
  manager.begin();
}

void loop()
{
  conn->tick();
  manager.tick();
  if (nextTick + 3000 < millis())
  {
    ContinueMessage msg;
    msg.shouldContinue = measuring;
    measuring = !measuring;
    nextTick = millis();
    Serial.print(msg.shouldContinue ? "yes" : "no");
    conn->send(msg);
  }
}

void onStatusChange(ConnectionState s)
{
  String msg;

  switch (s)
  {
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
