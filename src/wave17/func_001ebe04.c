struct LookupContext;
struct LookupEntry {
    const unsigned short *text;
};
extern const struct LookupEntry *bss_00290ec4[];

/* [INFERENCE] The first argument is a caller-supplied context object which this
 * table-backed implementation does not use; both indices are caller-evidenced. */
const unsigned short *func_001ebe04(const struct LookupContext *context,
                                  unsigned group, unsigned index)
{
    const struct LookupEntry *entry = bss_00290ec4[group] + index;
    return entry[1].text;
}
