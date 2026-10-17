struct AudioReadiness {
    unsigned char reserved00[0x0e];
    unsigned char ready;
    unsigned char generation;
};
struct AudioReadinessClock {
    unsigned char reserved00[0x1c];
    unsigned char active;
    unsigned char generation;
};
extern struct AudioReadinessClock bss_004b6584;

int func_0014de90(const struct AudioReadiness *state)
{
    if (!state->ready &&
        (!bss_004b6584.active ||
         (signed char)(bss_004b6584.generation - state->generation) > 2))
        return 0;
    return 1;
}
