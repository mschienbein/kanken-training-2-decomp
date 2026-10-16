/* [INFERENCE] Signature: Callee packet writes a word through r0, copies the byte mode from r1, and sends the aligned r2/r3 identifier; returns a command result word. */

extern unsigned func_0012d630(unsigned *out, unsigned char mode, unsigned long long id);

unsigned func_0012d62c(unsigned *out, unsigned char mode, unsigned long long id)
{
    return func_0012d630(out, mode, id);
}
