#include <SoftwareSerial.h>
#include "lib/SerialConnection.h"
#include "DualSerial.h"
#include "WifiManage.h"
#include "OnBoardSensor.h"

unsigned long nextTick = 0;
bool measuring = false;
WifiManager manager;
SerialConnection *conn = NULL;
DualSerial *dStream = new DualSerial();
OnBoardSensor sensors = OnBoardSensor();
PublishData publishData;

void onStatusChange(ConnectionState);
void onSensorMessage(SensorMessage);

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  conn = new SerialConnection(true, dStream);
  conn->onMessage(&onStatusChange);
  conn->onMessage(&onSensorMessage);
  conn->onMessage(&speedX);
  nextTick = millis();
  manager.begin();
}

void loop()
{
  conn->tick();
  manager.tick();
  sensors.tick();
  publishData.humidity = sensors.h;
  publishData.temperature = sensors.t;
  manager.setData(publishData);
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

void onSensorMessage(SensorMessage m)
{
  Serial.println();
  Serial.print(m.lightLevel);
  Serial.print('|');
  Serial.println(m.tick);
  publishData.lightLevel = m.lightLevel + 5;
  publishData.tick = m.tick;
}

void speedX(SpeedMessage spm)
{
  publishData.lightLevel = spm.speedLeft;
  Serial.print(spm.speedLeft);
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
