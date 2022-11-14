#include "Engine.h"
#include <Servo.h>

class Transmission
{
private:
  const byte dxFactor = 2;
  Engine *en1;
  Engine *en2;
  Servo servoMotor;
  byte speed = 0;
  // 90 makes tires straight
  byte angle = 90;

public:
  Transmission(Engine *eng1, Engine *eng2)
  {
    this->en1 = eng1;
    this->en2 = eng2;
    pinMode(9, OUTPUT);
    servoMotor.attach(9);
  }

  ~Transmission()
  {
    delete en1;
    delete en2;
  }

  void advance(byte sp, byte newAngle)
  {
    this->en1->moveforward();
    this->en2->moveforward();
    speed = sp;
    turn(newAngle);
  }

  void reverse(byte sp, byte newAngle)
  {
    this->en1->moveforward(false);
    this->en2->moveforward(false);
    speed = sp;
    turn(newAngle);
  }

private:
  void turn(byte newAngle)
  {
    angle = newAngle;
    servoMotor.write(angle);

    this->en1->setSpeed(speed);
    this->en2->setSpeed(speed);
    
    // enables closer turns around
    const byte dxSpeed = (speed / dxFactor);

    // turning right
    if(newAngle > 120){
      this->en1->setSpeed(dxSpeed);
    }

    // turning left
    if(newAngle < 60){
      this->en2->setSpeed(dxSpeed);
    }
    
  }
};
