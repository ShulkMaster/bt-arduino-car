#include "tones.h"
#include "Transmission.h"
#include "Lens.h"


int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};


unsigned long tracking = 0;

Transmission* transmission = NULL;
Lens* lens = NULL;

void setup(){
  Serial.begin(9600);
  Engine* en1 = new Engine(3,2,4);
  Engine* en2 = new Engine(5,6,7);
  transmission = new Transmission(en1, en2);
  lens = new Lens( 13, 12, 11);
  lens->registerCallback(&onMeasure);
  tracking = micros();
}

void onMeasure(int front, int back) {
  Serial.print("From last time ");
  Serial.println((micros() - tracking) / 1000);
  Serial.print("Measures in CM ");
  Serial.print(front);
  Serial.print(" ");
  Serial.println(back);
  tracking = micros();
  delay(250);
}

void loop(){
  /*
  byte dirs[] = {90, 90, 80, 70, 60, 50, 40, 90, 100, 120, 150, 160, 120 };
    for(int x = 0; x < 255; x= x + 5){
      byte dir = dirs[x % 13];
      transmission->advance(x, dir);
      delay(500);
    }
    */
    lens->tick();
}
