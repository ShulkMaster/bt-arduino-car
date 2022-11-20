#ifndef MESSAGE_H
#define MESSAGE_H

#define LENS_KIND 'L'
#define TEXT_KIND 'T'
#define READY_KIND 'K'
#define SPEED_KIND 'S'

struct Message {
    const byte Kind;
    
    Message(byte kind): Kind(kind) {}
};

struct LensMessage: Message {
    short frontD;
    short backD;
    LensMessage(): Message(LENS_KIND) {}
};

// 1byte padding
const short lensMessageSize =  sizeof(LensMessage) - 1;

struct TextMessage: Message {
    short lenght;
    char* chars;
    TextMessage(): Message(TEXT_KIND) {}
};

struct SpeedMessage: Message {
    short lenght;
    char* chars;
    TextMessage(): Message(TEXT_KIND) {}
};

#endif