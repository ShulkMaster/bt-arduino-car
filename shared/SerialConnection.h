#include "Serializer.h"
#include "Deserializer.h"

enum ConnectionState
{
    Disconected,
    Connecting,
    Connected,
    Incomming,
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

public:
    SerialConnection(bool start, Stream* ss) : starter(start)
    {
        this->m_serial = ss;
    }

    void onLensMessage(LensMsgCallback cb)
    {
        lensCb = cb;
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

    bool send(Message &m) {

    }

    private:
    void handleDisconnected(){

    }
};