class Engine
{
private:
  int controlPin;
  int forward;
  int backward;

public:
  Engine(int p1, int p2, int p3)
  {
    this->controlPin = p1;
    this->forward = p2;
    this->backward = p3;
    pinMode(controlPin, OUTPUT);
    digitalWrite(controlPin, LOW);
    pinMode(forward, OUTPUT);
    pinMode(backward, OUTPUT);
  }

  void stop()
  {
    analogWrite(controlPin, 0);
  }

  void moveforward(bool forwards = true)
  {
    int f1 = forwards ? HIGH : LOW;
    int f2 = forwards ? LOW : HIGH;
    digitalWrite(forward, f1);
    digitalWrite(backward, f2);
  }

  void setSpeed(short speed2)
  {
    analogWrite(controlPin, speed2);  
  }
};
