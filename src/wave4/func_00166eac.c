/* [INFERENCE] Signature: Callee packet forms a screen-slot address from r0 plus scaled r1 and calls DisplayObject helpers; r1 is an index, not a pointer. */

extern void func_00166eb0(void *screen, int index);

void func_00166eac(void *screen, int index)
{
    func_00166eb0(screen, index);
}
