struct RevisionState {
    unsigned char reserved00[0x50];
    unsigned enabled;
    int current;
    unsigned reserved58;
    int previous;
};

int func_001b4920(struct RevisionState *state)
{
    if (state->enabled && state->previous != state->current) {
        state->previous = state->current;
        return state->current;
    }
    return -1;
}
