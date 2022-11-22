#ifndef MESSAGE_H
#define MESSAGE_H

#define LENS_KIND 'W'
#define TEXT_KIND 'T'
#define CHALLENGE_KIND 'C'
#define CHALLENGE_RESPONSE_KIND 'Z'
#define SPEED_KIND 'S'

bool isMessageKind(byte b)
{
    return b == LENS_KIND 
    || b == TEXT_KIND 
    || b == CHALLENGE_KIND
    || b == CHALLENGE_RESPONSE_KIND
    || b == SPEED_KIND;
}

struct Message
{
    const byte Kind;
    Message(byte kind) : Kind(kind) {}
};

struct ChallengeMessage : Message
{
    char letter;
    short firts;
    short second;
    ChallengeMessage() : Message(CHALLENGE_KIND) {}
};

// 1 byte padding
const short challengeMessageSize = sizeof(ChallengeMessage);

struct ChallengeResponseMessage : Message
{
    char letter;
    short sum;
    ChallengeResponseMessage() : Message(CHALLENGE_RESPONSE_KIND) {}
};

const short challengeResponseMessageSize = sizeof(ChallengeResponseMessage);

struct LensMessage : Message
{
    short frontD;
    short backD;
    LensMessage() : Message(LENS_KIND) {}
};

const short lensMessageSize = sizeof(LensMessage);

struct TextMessage : Message
{
    short lenght;
    char *chars;
    TextMessage() : Message(TEXT_KIND) {}
};

struct SpeedMessage : Message
{
    short speedLeft;
    short speedRight;
    SpeedMessage() : Message(SPEED_KIND) {}
};

const short speedMessageSize = sizeof(SpeedMessage);

#endif