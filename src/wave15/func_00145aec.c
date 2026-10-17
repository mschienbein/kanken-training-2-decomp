struct PlaybackControl {
    unsigned reserved00;
    unsigned state;
};
extern struct PlaybackControl bss_00497508;

/* [INFERENCE] The single caller resets this state with zero. */
void func_00145aec(unsigned state)
{
    bss_00497508.state = state;
}
