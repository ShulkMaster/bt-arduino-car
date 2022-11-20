#include "Serializer.h"
#include "Deserializer.h"

#define ConnectionWaitTime 200
#define ConnectionWaitTimeout 250

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

    void tick()
    {
        switch (state)
        {
        case Disconected:
            handleDisconnected();
            break;
        }
    }

    template <typename MSG>
    bool send(MSG &m)
    {
        int size;
        byte *bts = serialize(m, size);
        int sent = m_serial->write(bts, size);
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
    void handleDisconnected()
    {
        if (track + ConnectionWaitTime > millis())
            return;
        state = Connecting;
        track = millis();
    }

    void handleConnecting()
    {
        if (starter)
        {
        }
    }
};