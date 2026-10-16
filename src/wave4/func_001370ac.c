/* [INFERENCE] Signature: Callee packet accesses the r0/r1 pointees, description offset 0x14 and a buffer/size command descriptor; unknown pointee layouts remain opaque and writable arguments are not const-qualified. */

extern unsigned func_001370b0(void *out, const void *description, void *buffer, unsigned size);

unsigned func_001370ac(void *out, const void *description, void *buffer, unsigned size)
{
    return func_001370b0(out, description, buffer, size);
}
