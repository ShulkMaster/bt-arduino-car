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