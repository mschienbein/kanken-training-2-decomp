/* [INFERENCE] Signature: Callee packet stores r0 in a selected context entry indexed by r1; pointer-vs-scalar for buffer inferred from graphics context use. */

extern void func_001229d4(void *buffer, unsigned index);

void func_001229d0(void *buffer, unsigned index)
{
    func_001229d4(buffer, index);
}
