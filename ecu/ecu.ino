#include <SoftwareSerial.h>
#include "lib/SerialConnection.h"
#include "DualSerial.h"

char letters[12] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'o', 'u', 't'};
unsigned long nextTick = 0;

SerialConnection *conn = NULL;
DualSerial* dStream = new DualSerial();
bool forward = true;

void onLensChange(LensMessage);

template <typename Func>
void vPrint(Func f)
{
  Serial.swap();
  f();
  Serial.swap();
}

void setup()
{
  Serial.begin(115200);
  Serial.swap();
  conn = new SerialConnection(true, dStream);
  conn->onMessage(&onLensChange);
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

void onLensChange(LensMessage lnsm)
{
  SpeedMessage spm;
  const short frontDistance = lnsm.frontD;
  const short backDistance = lnsm.backD;
  short newSpeed = 0;

  if (forward)
  {
    newSpeed = speedCalc(frontDistance);
  }
  else
  {
    newSpeed = speedCalc(backDistance);
  }
  vPrint([&frontDistance, &backDistance](){
    Serial.print("Lens recevied ");
    Serial.print(frontDistance);
    Serial.print(' ');
    Serial.println(backDistance);
  });
  conn->send(spm);
}
