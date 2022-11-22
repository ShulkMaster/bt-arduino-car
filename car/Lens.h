typedef void (*LensCallback)(short front, short back);

#define WAIT_TIME 20
#define PULSE_TIMEOUT 10000

enum LensState {
  PulseF,
  MeasureF,
  PulseB,
  MeasureB,
};

class Lens {
  private:
  const byte  triggerF;
  const byte echoFront;
  const byte  triggerB;
  const byte echoBack;
  LensCallback cb = NULL;
  unsigned long lastTime = 0;
  LensState state = PulseF;
  short temp = 0;

public:
  Lens(byte trigF, byte trigB, byte echoF,byte echoB): triggerF(trigF), triggerB(trigB), echoFront(echoF), echoBack(echoB) {
    pinMode(trigF, OUTPUT);
    pinMode(trigB, OUTPUT);
    pinMode(echoF, INPUT);
    pinMode(echoB, INPUT);
    digitalWrite(trigF, LOW);
    digitalWrite(trigB, LOW);
  }

  void registerCallback(LensCallback cb){
    this->cb = cb;
  }

  void tick(){
    switch(state){
       case PulseF:
         if(lastTime + WAIT_TIME > micros()) return;
         digitalWrite(triggerF, HIGH);
         state = MeasureF;
         lastTime =  micros();
       case MeasureF:
         if(lastTime + WAIT_TIME > micros()) return;
         digitalWrite(triggerF, LOW);
         temp = pulseIn(echoFront, HIGH, PULSE_TIMEOUT) / 59;
         state = PulseB;
         lastTime =  micros();
       case PulseB:
         if(lastTime + WAIT_TIME > micros()) return;
         digitalWrite(triggerB, HIGH);
         state = MeasureB;
         lastTime =  micros();
       case MeasureB:
         if(lastTime + WAIT_TIME > micros()) return;
         digitalWrite(triggerB, LOW);
         short backDistance = pulseIn(echoBack, HIGH, PULSE_TIMEOUT) / 59;
         cb(temp, backDistance);
         state = PulseF;
         lastTime =  micros();
      }
  }

};
