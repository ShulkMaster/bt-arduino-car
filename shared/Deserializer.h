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
    byte index = 2;
    m.firts = fromBigEndian(index, buff);
    index+= sizeof(short);
    m.second = fromBigEndian(index, buff);
}

void deserialize(ChallengeResponseMessage &m, byte *buff) {
    m.letter = buff[1];
    m.sum = fromBigEndian(2, buff);
}

void deserialize(SpeedMessage &m, byte *buff) {
    m.speeedLeft = fromBigEndian(1, buff);
    m.speeedRight = fromBigEndian(1 + sizeof(SpeedMessage::speeedLeft), buff);
}