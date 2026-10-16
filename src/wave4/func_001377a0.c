/* [INFERENCE] Signature: Callee packet consumes all four incoming general registers while assembling a graphics command; r2 is compared as a mode. */

extern void func_001377a4(unsigned kind, unsigned flags, int mode, unsigned value);

void func_001377a0(unsigned kind, unsigned flags, int mode, unsigned value)
{
    func_001377a4(kind, flags, mode, value);
}
