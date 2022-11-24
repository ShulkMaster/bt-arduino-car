#include "Engine.h"
#include "lib/Message.h"

class Transmission
{
private:
  Engine *en1;
  Engine *en2;

public:
  Transmission(Engine *eng1, Engine *eng2)
  {
    this->en1 = eng1;
    this->en2 = eng2;
  }

  ~Transmission()
  {
    delete en1;
    delete en2;
  }

  void move(SpeedMessage spm)
  {
    en1->setSpeed(abs(spm.speedLeft));
    en1->moveforward(spm.speedLeft >= 0);

    en2->setSpeed(abs(spm.speedRight));
    en2->moveforward(spm.speedRight >= 0);
  }

  void stop(){
    en1->stop();
    en2->stop();
  }
};
