struct ProgressCounters {
    unsigned char reserved00[0x70];
    int first_limit;
    int first;
    unsigned char reserved78[0x18];
    int second_limit;
    int second;
    unsigned char reserved98[0x28];
    int third_limit;
    int third;
};

void func_001d2be8(struct ProgressCounters *state)
{
    if (state->first < state->first_limit)
        ++state->first;
    if (state->second < state->second_limit)
        ++state->second;
    if (state->third < state->third_limit)
        ++state->third;
}
