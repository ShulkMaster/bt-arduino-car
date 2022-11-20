#ifndef MESSAGE_H
#define MESSAGE_H

#define LENS_KIND 'L'
#define TEXT_KIND 'T'
#define CHALLENGE_KIND 'C'
#define CHALLENGE_RESPONSE_KIND 'Z'
#define SPEED_KIND 'S'

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
const short challengeMessageSize = sizeof(ChallengeMessage) - 1;

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

// 1byte padding
const short lensMessageSize = sizeof(LensMessage) - 1;

struct TextMessage : Message
{
    short lenght;
    char *chars;
    TextMessage() : Message(TEXT_KIND) {}
};

struct SpeedMessage : Message
{
    short speeedLeft;
    short speeedRight;
    SpeedMessage() : Message(SPEED_KIND) {}
};

// 1byte padding
const short speedMessageSize = sizeof(SpeedMessage) - 1;

#endif