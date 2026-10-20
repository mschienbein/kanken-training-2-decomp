/* Matches the declaration in src/pilot/pilot_a.c. */
extern int func_00122350(int i);

static unsigned align_up_4(unsigned value)
{
    return (value + 3U) & ~3U;
}

/* [INFERENCE] The caller uses this word as an allocation size: a packed bitmap
 * rounded to words, 44-byte records per item and func_00122350 words per item. */
unsigned func_0010c504(unsigned count)
{
    return align_up_4((count + 7U) >> 3) + count * 11U * 4U +
           (unsigned)func_00122350(count) * 4U + 32U;
}
