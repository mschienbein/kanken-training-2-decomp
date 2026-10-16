/* [INFERENCE] Signature: Callee packet writes a byte through r0, sends byte mode plus aligned r2/r3 identifier, and returns the command result. */

extern unsigned func_001363b4(unsigned char *out, unsigned char mode, unsigned long long id);

unsigned func_001363b0(unsigned char *out, unsigned char mode, unsigned long long id)
{
    return func_001363b4(out, mode, id);
}
