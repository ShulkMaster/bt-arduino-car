#include "Message.h"

short fromBigEndian(short offset, byte* buff) {
  short value = 0;
  value = (value << 8) + buff[offset];
  value = (value << 8) + buff[offset + 1];
  return value;
}

void deserialize(LensMessage &m, byte *buff)
{
    //skip firts header byte
    m.frontD = fromBigEndian(1, buff);
    m.backD = fromBigEndian(3, buff);
}

void deserialize(ChallengeMessage &m, byte *buff) {
    m.letter = buff[1];
    m.firts = fromBigEndian(2, buff);
    m.second = fromBigEndian(4, buff);
}

void deserialize(ChallengeResponseMessage &m, byte *buff) {
    m.letter = buff[1];
    m.sum = fromBigEndian(2, buff);
}

void deserialize(SpeedMessage &m, byte *buff) {
    m.speedLeft = fromBigEndian(1, buff);
    m.speedRight = fromBigEndian(3, buff);
}

void deserialize(ContinueMessage &m, byte *buff) {
    m.shouldContinue = buff[1];
}

void deserialize(SensorMessage &m, byte *buff){
    m.lightLevel = fromBigEndian(1, buff);
    m.tick = fromBigEndian(3, buff);
}