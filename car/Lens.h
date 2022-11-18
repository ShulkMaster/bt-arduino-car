typedef void (*LensCallback)(int front, int back);

#define BAD_STATE -69
#define WAIT_TIME 20
#define PULSE_TIMEOUT 400000

enum LensState {
  Idle,
  LowPulse,
  MeasuringF,
  MeasuringB,
};

class Lens {
  private:
  // trigger pin is shared
  const byte  triggerPin;
  const byte echoFront;
  const byte echoBack;
  LensCallback cb = NULL;
  unsigned long lastTime = 0;
  LensState state = Idle;
  int temp = BAD_STATE;

public:
  Lens(byte trig, byte echoF,byte echoB): triggerPin(trig), echoFront(echoF), echoBack(echoB) {
    pinMode(trig, OUTPUT);
    pinMode(echoF, INPUT);
    pinMode(echoB, INPUT);
  }

  void registerCallback(LensCallback cb){
    this->cb = cb;
  }

  void tick(){
    switch(state){
      case Idle:
        if(lastTime + 5500 > micros())return;
        lastTime = micros();
        state = LowPulse;
        digitalWrite(triggerPin, LOW);
        return;
       case LowPulse:
       if(lastTime + 10 > micros())return;
        state = temp == BAD_STATE ? MeasuringF : MeasuringB;
        digitalWrite(triggerPin, HIGH);
        lastTime = micros();
        return;
       case MeasuringF:
       if(lastTime + WAIT_TIME > micros())return;
       state = Idle;
       temp = pulseIn(echoFront, HIGH, PULSE_TIMEOUT) / 59;
       return;
       case MeasuringB:
       if(lastTime + WAIT_TIME > micros())return;
       int backD = pulseIn(echoBack, HIGH, PULSE_TIMEOUT) / 59;
       cb(temp, backD);
       temp = BAD_STATE;
       state = Idle;
      }
  }

};
