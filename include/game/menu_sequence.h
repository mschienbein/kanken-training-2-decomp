#ifndef GAME_MENU_SEQUENCE_H
#define GAME_MENU_SEQUENCE_H



#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */
struct W19aMenuSequence;

struct W19aMenuSequence {
    unsigned char reserved00[0x90];
    unsigned current;
    unsigned reserved94;
    unsigned entries[9];

    void set_entry(unsigned index, unsigned value)
    {
        entries[index] = value;
    }
};

extern "C" {
void func_001e3050(W19aMenuSequence *state);
}
#endif

#endif
