/* [INFERENCE] Signature: Callee packet uses r0 as screen base, r1 as slot index and r2 as enabled/mode for widget-update helpers. */

extern void func_00166f20(void *screen, int index, int enabled);

void func_00166f1c(void *screen, int index, int enabled)
{
    func_00166f20(screen, index, enabled);
}
