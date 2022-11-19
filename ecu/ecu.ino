#include <SoftwareSerial.h>

char letters[12] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'o', 'u', 't'};

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
}

void loop() {
  int mi = millis() / 1000;
  delay(2000);
  byte dataInt[4] = {0};
  byte dataChar[4] = {0};
  for(int i = 0; i < 4; i++){
    int index = random(0, 12);
    dataInt[3-i] = (mi >> (i * 8));
    dataChar[i] = letters[index];
  }
  Serial1.write(dataInt, 4);
  Serial1.write(dataInt, 4);
}
