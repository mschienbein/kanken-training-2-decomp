struct W23JProgressDay {
    unsigned char flags;
    unsigned char reserved01[3];
    unsigned date;
};
struct W23JProgressStore {
    unsigned char reserved00[0x62a0];
    struct W23JProgressDay days[];
};
/* INTEGRATOR: differs from include/game/notification.h: the reserved region
 * contains eight-byte day records with byte flags at absolute offset 0x62a0. */
extern struct W23JProgressStore bss_00280af0;
extern int func_001699b8(void);

void func_001dc1ac(void)
{
    bss_00280af0.days[func_001699b8()].flags |= 4;
}
