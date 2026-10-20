/* The caller supplies a writable object and discards the result.
 * [INFERENCE] Concrete object type and residual-count return contract. */
extern "C" int func_001c1bdc(void *object)
{
    int i;
    for (i = 0; i < 9; ++i) {
    }
    return i;
}
