/* INTEGRATOR: differs from include/game/selection.h: extends the evidenced
 * SelectionState prefix with signed halfword counters at 0x58 and 0x5a. */
struct SelectionState {
    unsigned char reserved00[0x52];
    short selection;
    short mode;
    unsigned char reserved56[2];
    short attempts;
    short successes;
};
extern struct SelectionState bss_0028c1f0;

int func_001dc160(void)
{
    return bss_0028c1f0.successes;
}
