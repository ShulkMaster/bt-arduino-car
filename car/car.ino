#include "tones.h"
#include <Servo.h>
#include "Engine.h"

int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

Servo servoMotor;
Engine* en1 = NULL;

void setup(){
  pinMode(9, OUTPUT);
  servoMotor.attach(9);
  Serial.begin(9600);
  en1 = new Engine(3,2,4);
}

void loop(){
    servoMotor.write(0);
    en1->moveforward();
    delay(1000);
    servoMotor.write(90);
    en1->moveforward(false);
    delay(1000);
    servoMotor.write(180);
    en1->stop();
    delay(1300);
}
