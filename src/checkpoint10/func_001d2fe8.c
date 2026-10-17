#include <stdbool.h>

struct TimedCounter {
    unsigned int reserved00[2];
    int limit, current;
};
struct ExtendedTimedState {
    unsigned char reserved00[0x68];
    struct TimedCounter first;
    unsigned char reserved78[0x10];
    struct TimedCounter second;
    unsigned char reserved98[0x20];
    struct TimedCounter third;
    unsigned char reservedc8[0x3048 - 0xc8];
    struct TimedCounter channels[4];
    unsigned char reserved3088[0x328c - 0x3088];
    unsigned short enabled;
};

extern void func_001d2be8(struct ExtendedTimedState *state);

static __forceinline void advance_counter(struct TimedCounter *counter)
{
    if (counter->current < counter->limit)
        ++counter->current;
}

void func_001d2fe8(struct ExtendedTimedState *state)
{
    unsigned short enabled;
    func_001d2be8(state);
    enabled = state->enabled;
    if (enabled & 1)
        advance_counter(&state->channels[0]);
    enabled >>= 1;
    if (enabled & 1)
        advance_counter(&state->channels[1]);
    enabled >>= 1;
    if (enabled & 1)
        advance_counter(&state->channels[2]);
    {
        bool last_enabled = (enabled >> 1) & 1;
        if (last_enabled)
            advance_counter(&state->channels[3]);
    }
}
