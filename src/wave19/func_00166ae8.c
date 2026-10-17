struct ScreenTransition {
    unsigned char reserved00[0x34];
    int mode;
    unsigned char reserved38[0x58];
    int current_slot;
    int current[2];
    int previous[2];
    int pending;
    unsigned char reserveda8[8];
    int notification_mode;
};

void func_00166ae8(struct ScreenTransition *state, int value, int slot)
{
    if (slot == -1) {
        state->previous[state->current_slot] = state->current[state->current_slot];
        state->current[state->current_slot] = value;
    } else {
        state->previous[slot] = state->current[slot];
        state->current[slot] = value;
    }
    state->pending = 0;
}
