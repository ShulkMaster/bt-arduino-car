#include "Serializer.h"
#include "Deserializer.h"

#define ConnectionWaitTime 2000
#define ConnectionWaitTimeout 2500
#define FIRST_LETTER 'a'
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

public:
    SerialConnection(bool start, Stream *ss) : starter(start)
    {
        this->m_serial = ss;
    }

    ConnectionState getState() {
        return state;
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
            if(track + ConnectionWaitTime > millis())
            return;
            setState(Disconected);
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
            m.letter = LETTER_OK;
            m.firts = -1;
            m.second = 1;
            send(m);
        }
    }

    void handleConnecting()
    {
        byte span = m_serial->peek();
        if (span > -1 && span == CHALLENGE_KIND)
        {
            if(m_serial->available() < challengeMessageSize) return;
            ChallengeMessage m;
            m_serial->println("SI");
            byte buff[challengeMessageSize] = {0};
            m_serial->readBytes(buff, challengeMessageSize);
            deserialize(m, buff);
            m_serial->print(m.letter);
            m_serial->print(' ');
            m_serial->print(m.firts);
            m_serial->print(' ');
            m_serial->println(m.second);
            if(m.letter = LETTER_OK && m.firts - m.second == -2){
                setState(Connected);
                return;
            }
            ChallengeResponseMessage response;
            response.letter = m.letter == FIRST_LETTER ? FIRST_LETTER_REPLY : ':';
            response.sum = m.firts + m.second;
            send(response);
            return;
        }
        m_serial->read();
    }

    void handleConnectingStart()
    {
        if (millis() > track + ConnectionWaitTimeout)
        {
            setState(Timeout);
            return;
        }

        if (m_serial->available() >= challengeResponseMessageSize)
        {
            ChallengeResponseMessage m;
            byte *buff;
            m_serial->readBytes(buff, challengeResponseMessageSize);
            deserialize(m, buff);
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
    }
};