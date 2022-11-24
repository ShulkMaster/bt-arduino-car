#include "Serializer.h"
#include "Deserializer.h"

#define ConnectionWaitTime 500
#define ConnectionWaitTimeout 700
#define FIRST_LETTER 'b'
#define FIRST_LETTER_REPLY 'e'
#define LETTER_OK '+'

enum ConnectionState
{
  Disconected,
  Connecting,
  Connected,
  Incomming,
  Timeout,
  Invalid,
};

typedef void (*LensMsgCallback)(LensMessage msg);
typedef void (*StatusCallback)(ConnectionState msg);
typedef void (*SpeedCallback)(SpeedMessage msg);

class SerialConnection
{
private:
  ConnectionState state = Disconected;
  const bool starter;
  Stream *m_serial;
  LensMsgCallback lensCb = NULL;
  SpeedCallback speedCb = NULL;
  StatusCallback statusCb = NULL;
  unsigned long track = 0;
  unsigned long lastIncomingMsg = 0;

public:
  SerialConnection(bool start, Stream *ss) : starter(start)
  {
    this->m_serial = ss;
  }

  ConnectionState getState()
  {
    return state;
  }

  unsigned long lastReceived()
  {
    return lastIncomingMsg;
  }

  void tick()
  {
    switch (state)
    {
    case Disconected:
      handleDisconnected();
      break;
    case Connecting:
      if (starter)
      {
        handleConnectingStart();
      }
      else
        handleConnecting();
      break;
    case Timeout:
    case Invalid:
      if (track + ConnectionWaitTime > millis())
        return;
      setState(Disconected);
    case Connected:
      handleConnected();
      break;
    case Incomming:
      handleIncoming();
      break;
    }
  }

  template <typename MSG>
  bool send(MSG &m)
  {
    int size;
    byte *bts = serialize(m, size);
    int sent = m_serial->write(bts, size);
    delete bts;
    return size == sent;
  }

  void onMessage(LensMsgCallback cb)
  {
    lensCb = cb;
  }

  void onMessage(SpeedCallback cb)
  {
    speedCb = cb;
  }

  void onMessage(StatusCallback cb)
  {
    statusCb = cb;
  }

private:
  void setState(ConnectionState s)
  {
    if(state == Incomming && s == Connected){
      lastIncomingMsg = millis();
    }
    state = s;
    statusCb(state);
    track = millis();
  }

  void handleDisconnected()
  {
    if (track + ConnectionWaitTime > millis())
      return;
    setState(Connecting);
    if (starter)
    {
      ChallengeMessage m;
      m.letter = FIRST_LETTER;
      m.firts = 50;
      m.second = 19;
      send(m);
    }
  }

  void handleConnecting()
  {
    byte span = m_serial->peek();
    if (span == CHALLENGE_KIND)
    {
      if (m_serial->available() < challengeMessageSize)
        return;
      ChallengeMessage m;
      byte buff[challengeMessageSize] = {0};
      m_serial->readBytes(buff, challengeMessageSize);
      deserialize(m, buff);

      if (m.letter == LETTER_OK)
      {
        setState(Connected);
        return;
      }

      ChallengeResponseMessage response;
      response.letter = m.letter == FIRST_LETTER
                            ? FIRST_LETTER_REPLY
                            : '$';
      response.sum = m.firts + m.second;
      send(response);
      return;
    }
    m_serial->read();
  }

  void handleConnectingStart()
  {
    if (millis() > track + ConnectionWaitTimeout + 20)
    {
      setState(Timeout);
      return;
    }

    byte span = m_serial->peek();
    if (span == CHALLENGE_RESPONSE_KIND)
    {
      if (m_serial->available() < challengeResponseMessageSize)
        return;
      ChallengeResponseMessage m;
      byte *buff = new byte[challengeResponseMessageSize];
      m_serial->readBytes(buff, challengeResponseMessageSize);
      deserialize(m, buff);
      delete buff;

      if (m.letter == FIRST_LETTER_REPLY && m.sum == 69)
      {
        ChallengeMessage confirmation;
        confirmation.letter = LETTER_OK;
        confirmation.firts = -1;
        confirmation.second = 1;
        send(confirmation);
        setState(Connected);
        return;
      }
      setState(Invalid);
    }
    m_serial->read();
  }

  void handleConnected()
  {
    byte span = m_serial->peek();
    if (isMessageKind(span))
    {
      setState(Incomming);
      return;
    }
    m_serial->read();
  }

  void handleIncoming()
  {
    byte *buff = NULL;
    switch (m_serial->peek())
    {
    case LENS_KIND:
      if (m_serial->available() >= lensMessageSize)
      {
        LensMessage m;
        buff = new byte[lensMessageSize];
        m_serial->readBytes(buff, lensMessageSize);
        deserialize(m, buff);
        lensCb(m);
        setState(Connected);
      }
      break;
    case SPEED_KIND:
      if (m_serial->available() >= speedMessageSize)
      {
        SpeedMessage m;
        buff = new byte[speedMessageSize];
        m_serial->readBytes(buff, speedMessageSize);
        deserialize(m, buff);
        speedCb(m);
        setState(Connected);
      }
      break;
    case CHALLENGE_KIND:
      break;
    case CHALLENGE_RESPONSE_KIND:
      break;
    default:
      break;
    }
    delete buff;
  }

  ChallengeResponseMessage accepted(ChallengeMessage challenge)
  {
    ChallengeResponseMessage response;
    response.letter = challenge.letter == FIRST_LETTER
                          ? FIRST_LETTER_REPLY
                          : '$';
    response.sum = challenge.firts + challenge.second;
    return response;
  }

  void checkChallenge(ChallengeResponseMessage &m)
  {
    if (m.letter == FIRST_LETTER_REPLY && m.sum == 76)
    {
      setState(Connected);
      ChallengeMessage m;
      m.letter = LETTER_OK;
      m.firts = -1;
      m.second = 1;
      send(m);
      return;
    }
    setState(Invalid);
  }
};