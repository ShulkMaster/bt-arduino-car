class Engine
{
private:
  int controlPin;
  int forward;
  int backward;
  byte speed;

public:
  Engine(int p1, int p2, int p3)
  {
    this->controlPin = p1;
    this->forward = p2;
    this->backward = p3;
    this->speed = 0;
    pinMode(controlPin, OUTPUT);
    digitalWrite(controlPin, LOW);
    pinMode(forward, OUTPUT);
    pinMode(backward, OUTPUT);
  }

  void stop()
  {
    speed = 0;
    analogWrite(controlPin, speed);
  }

  void moveforward(bool forwards = true)
  {
    digitalWrite(forward, forwards);
    digitalWrite(backward, !forwards);
    analogWrite(controlPin, speed);
  }

  void setSpeed(byte speed)
  {
    this->speed = speed;
    analogWrite(controlPin, speed);
  }
};
