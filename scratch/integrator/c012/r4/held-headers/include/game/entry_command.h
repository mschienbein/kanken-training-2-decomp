#ifndef GAME_ENTRY_COMMAND_H
#define GAME_ENTRY_COMMAND_H
#include "game/command_state.h"
#ifdef __cplusplus
/* Names-admission func_0015f84c proves this full constructor. [INFERENCE]
 * Entry-command names and Reset grouping describe its observed stores. */
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
#endif
#endif
