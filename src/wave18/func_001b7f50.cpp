/* Callers supply a writable object and discard the result.
 * [INFERENCE] Concrete object type and residual-count return contract. */
extern "C" int func_001b7f50(void *object)
{
    int i;
    for (i = 0; i < 4; ++i) {
    }
    return i;
}
