#include "game/command_state.h"

/* [INFERENCE] Entry-command layout and the Reset() helper are recovered from the constructor stores. */
struct W11EntryCommand : W11BaseCommand {
    W11EntryCommand();
    void Reset();
    virtual ~W11EntryCommand();
    short resource;
    signed char kind;
    signed char mode;
    signed char option;
    unsigned char reserved09[3];
    void *first;
    void *second;
    unsigned char active;
    unsigned char reserved15[3];
    unsigned count;
    unsigned capacity;
};

inline void W11EntryCommand::Reset()
{
    resource = -1;
    kind = -1;
    mode = -1;
    option = -1;
    first = 0;
    second = 0;
    active = 0;
    count = 0;
    capacity = 0;
}

W11EntryCommand::W11EntryCommand()
{
    Reset();
}
