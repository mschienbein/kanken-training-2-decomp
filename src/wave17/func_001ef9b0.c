struct IntervalState {
    unsigned reserved00[2];
    unsigned long long begin;
    unsigned long long end;
};

/* [INFERENCE] Unsigned tick values; the subtraction itself cannot distinguish
 * signed from unsigned intervals. Both output words are written. */
int func_001ef9b0(const struct IntervalState *state, unsigned long long *duration)
{
    *duration = state->end - state->begin;
    return 0;
}
