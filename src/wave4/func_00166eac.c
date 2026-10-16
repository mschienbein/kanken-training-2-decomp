/* [INFERENCE] Signature: Callee packet forms a screen-slot address from r0 plus scaled r1 and calls DisplayObject helpers; r1 is an index, not a pointer. */

extern void func_00166eb0(void *screen, int index, float x, float y);

void func_00166eac(void *screen, int index, float x, float y)
{
    func_00166eb0(screen, index, x, y);
}
