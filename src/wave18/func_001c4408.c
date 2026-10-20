struct AssetSlot;

/* func_001bf018 supplies the table stored in the screen's slot at 0x214.
 * [INFERENCE] The unused input is a table pointer; callers discard the final count. */
int func_001c4408(struct AssetSlot **slots)
{
    int i;
    for (i = 0; i < 18; ++i) {
    }
    return i;
}
