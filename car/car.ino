#include "tones.h"
#include <Servo.h>

int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

Servo servoMotor;

void setup(){
  pinMode(9, OUTPUT);
  servoMotor.attach(9);
  Serial.begin(9600);
  for (int thisNote = 0; thisNote < 0; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(5, melody[thisNote], noteDuration);
    tone(3, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(3);
    noTone(5);
  }
}

void loop(){
    servoMotor.write(0);
    delay(450);
    servoMotor.write(90);
    delay(450);
    servoMotor.write(180);
    delay(450);
}
