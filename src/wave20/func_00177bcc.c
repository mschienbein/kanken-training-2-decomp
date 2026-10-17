struct CategoryTextCatalog;
struct CategoryValues {
    unsigned reserved00;
    const unsigned short *values[];
};
extern const struct CategoryValues *bss_00290ec4[];
extern int func_001dd3a4(int category, int kind);

/* [INFERENCE] The unused first input is the caller's category/context object. */
/* The returned pointer is passed as UTF-16 text to func_0017a43c by both callers. */
const unsigned short *func_00177bcc(const struct CategoryTextCatalog *context, int category, int kind)
{
    const struct CategoryValues *values = bss_00290ec4[category];
    int index = func_001dd3a4(category, kind);
    /* ORIGINAL BUG: a missing kind produces index -1, selecting the prefix word. */
    return values->values[index];
}
