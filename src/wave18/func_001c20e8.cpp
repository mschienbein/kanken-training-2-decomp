struct W18DisplayCollection;

/* Callers pass a writable ten-slot collection and ignore the result.
 * [INFERENCE] The residual counter is exposed as the return value. */
extern "C" int func_001c20e8(W18DisplayCollection *collection)
{
    int i;
    for (i = 0; i < 10; ++i) {
    }
    return i;
}
