/* [INFERENCE] Identity: a fill over the half-open range [begin, end) of 32-bit
 * words (the SDK's std::fill-style loop over by-value one-pointer iterators).
 * The iterators' storage is addressed through the Cursor accessor, which keeps
 * both pointers memory-resident and reloaded each iteration like the original;
 * the type name WordIterator is not a recovered identity. */
struct WordIterator {
    unsigned *current;
};

static __inline unsigned **Cursor(struct WordIterator *iterator)
{
    return &iterator->current;
}

void func_00121cf8(struct WordIterator begin, struct WordIterator end,
                   unsigned value)
{
    while (*Cursor(&begin) != *Cursor(&end)) {
        unsigned *old = *Cursor(&begin);
        *Cursor(&begin) = old + 1;
        *old = value;
    }
}
