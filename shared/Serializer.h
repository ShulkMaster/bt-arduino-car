#include "Message.h"

void toBigEndian(int value, short offset, byte* buff) {
  buff[offset] = value >> 8;
  buff[offset + 1] = value;
}

byte* serialize(ChallengeMessage &m, int &size) {
    size = challengeMessageSize;
    byte* data = new byte[size];
    data[0] = m.Kind;
    data[1] = m.letter;
    byte index = 2;
    toBigEndian(m.firts, index, data);
    index+= sizeof(ChallengeMessage::firts);
    toBigEndian(m.second, index, data);
    return data;
}

byte* serialize(ChallengeResponseMessage &m, int &size) {
    size = challengeResponseMessageSize;
    byte* data = new byte[size];
    data[0] = m.Kind;
    data[1] = m.letter;
    toBigEndian(m.sum, 2, data);
    return data;
}

byte* serialize(SpeedMessage &m, int &size) {
    size = speedMessageSize;
    byte* data = new byte[size];
    data[0] = m.Kind;
    toBigEndian(m.speedLeft, 1, data);
    toBigEndian(m.speedRight, 3, data);
    return data;
}

byte* serialize(LensMessage &m, int &size) {
    size = lensMessageSize;
    byte* data = new byte[size];
    data[0] = m.Kind;
    toBigEndian(m.frontD, 1, data);
    toBigEndian(m.backD, 3, data);
    return data;
}