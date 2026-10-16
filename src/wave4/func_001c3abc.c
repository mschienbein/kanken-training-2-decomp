/* [INFERENCE] Signature: Callee packet consumes r0-r3: screen/table base, slot index, stored value and compared mode; all four are forwarded. */

extern void func_001c3ac0(void *screen, int index, unsigned value, int mode);

void func_001c3abc(void *screen, int index, unsigned value, int mode)
{
    func_001c3ac0(screen, index, value, mode);
}
