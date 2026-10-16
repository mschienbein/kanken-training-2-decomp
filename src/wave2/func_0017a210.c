struct AnimationState {
    unsigned char reserved00[0x44];
    float start;
    float finish;
    unsigned char reserved4c[0x0c];
    float lower;
    float upper;
    float current;
};

int func_0017a210(const struct AnimationState *state)
{
    if (state->finish == state->start)
        return 0;
    if (state->current == state->lower)
        return 2;
    if (state->current == state->upper)
        return 3;
    return 1;
}
