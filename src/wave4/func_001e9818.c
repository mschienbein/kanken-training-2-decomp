/* [INFERENCE] Signature: Callee packet uses r0 as screen base, scaled r1 as a slot index and stores/tests incoming r2/r3. */

extern void func_001e981c(void *screen, int index, int first, int second);

void func_001e9818(void *screen, int index, int first, int second)
{
    func_001e981c(screen, index, first, second);
}
