#include "Message.h"

void toBigEndian(int value, short offset, byte* buff) {
  buff[offset] = value >> 8;
  buff[offset + 1] = value;
}

byte* serialize(LensMessage &m, int &size) {
    size = lensMessageSize;
    byte* data = new byte[size];
    byte index = 0;
    data[index] = m.Kind;
    index++;
    toBigEndian(m.frontD, index, data);
    index+= 2;
    toBigEndian(m.backD, index, data);
    return data;
}