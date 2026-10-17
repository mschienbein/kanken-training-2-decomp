/* INTEGRATOR: differs from include/game/screen.h: the global's partial
 * DisplayQueueCounter layout also has an enable word at +0x0c. */
struct DisplayQueueCounter {
    unsigned char reserved00[8];
    int count;
    int enabled;
};

extern struct DisplayQueueCounter bss_00495c70;

void func_0010a814(void)
{
    struct DisplayQueueCounter *state = &bss_00495c70;
    state->enabled = 1;
}
