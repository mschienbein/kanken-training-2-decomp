#include "game/notification.h"

static unsigned int add_eligible_grade(int bit, int grade, unsigned int mask, unsigned int unit)
{
    if (grade > bit)
        mask |= unit << bit;
    return mask;
}

static unsigned int build_grade_mask(unsigned int mask, const struct SavedNotification *saved, unsigned int unit)
{
    int grade = (short)saved->payload.storage.text[0x18];
    int bit;
    /* A signed -1 selector enables all twelve grades; this is not a parity flag. */
    if (grade == -1) {
        for (bit = 0; bit < 12; ++bit)
            mask |= unit << (11 - bit);
    } else {
        bit = 11;
        for (;;) {
            mask = add_eligible_grade(bit, grade, mask, unit);
            --bit;
            if (bit < 0)
                break;
        }
    }
    return mask;
}

unsigned int func_0017418c(void)
{
    return build_grade_mask(0, &bss_00280af0, 1u);
}
