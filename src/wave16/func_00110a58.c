/* [INFERENCE] Prefix of scheduler task zero. func_0019f338 establishes
 * the callback at +8 and resets the following counter at +12. */
struct SchedulerTaskPrefix {
    unsigned char reserved00[8];
    void (*callback)(void);
    unsigned counter;
};

extern struct SchedulerTaskPrefix bss_00495c8c;

void func_00110a58(void)
{
    bss_00495c8c.callback = 0;
    bss_00495c8c.counter = 0;
}
