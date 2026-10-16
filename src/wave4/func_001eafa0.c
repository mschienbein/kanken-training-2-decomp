/* [INFERENCE] Signature: Callee packet uses r0/r1 as screen/slot index and forwards r2 as UTF-16 text to func_00170c20; helper packet and AuthorPilotA/func_00170c20.cpp:66-67 corroborate text type. Saved incoming r3 is padding, not a parameter. */

extern void func_001eafa4(void *screen, int index, const unsigned short *text);

void func_001eafa0(void *screen, int index, const unsigned short *text)
{
    func_001eafa4(screen, index, text);
}
