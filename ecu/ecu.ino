#include <SoftwareSerial.h>
#include "lib/Serializer.h"
#include "lib/Deserializer.h"

char letters[12] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'o', 'u', 't'};
unsigned long nextTick = 0;


void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  delay(250);
  Serial.println();
  LensMessage m;
  m.frontD = 75;
  m.backD = 42;
  int sizeBuff = 0;
  byte *data = serialize(m, sizeBuff);
  Serial.print("size: ");
  Serial.println(sizeBuff);
  byte x = (char)data[0];
  LensMessage m2;
  switch(x){
    case LENS_KIND: 
    Serial.print("LENS_KIND ");
    deserialize(m2, data);
    Serial.print(m2.frontD);
    Serial.print(' ');
    Serial.println(m2.backD);
    break;
    case TEXT_KIND:
    Serial.print("TEXT_KIND ");
    break;
    }
  delete[] data;
}

void loop() {
  return;
  if(Serial.available() >= 8){
    int received = 0;
    byte intReceived[4] = {0};
    Serial.readBytes(intReceived, 4);
    for(int x = 0; x < 4; x++){
      received = (received << 8*x) + intReceived[x];
    }
    Serial.print("front ");
    Serial.println(received);
    received = 0;
    Serial.readBytes(intReceived, 4);
    for(int x = 0; x < 4; x++){
      received = (received << 8*x) + intReceived[x];
    }
    Serial.print("back ");
    Serial.println(received);
  }
  if(nextTick + 2000 > millis()) return;
  nextTick = millis();
  int mi = nextTick / 1000;
  byte dataInt[4] = {0};
  byte dataChar[4] = {0};
  Serial.print("sending ");
  for(int i = 0; i < 4; i++){
    int index = random(0, 12);
    dataInt[3-i] = (mi >> (i * 8));
    dataChar[i] = letters[index];
    Serial.print((char) dataChar[i]);
  }
  Serial.println();
  //Serial1.write(dataInt, 4);
  Serial1.write(dataChar, 4);
                   
  
}
