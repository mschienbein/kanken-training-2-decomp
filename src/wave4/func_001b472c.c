/* [INFERENCE] Signature: Callee packet uses r0 as screen/object base and r1 as an index into global and widget tables. */

extern void func_001b4730(void *screen, int index);

void func_001b472c(void *screen, int index)
{
    func_001b4730(screen, index);
}
