/* [INFERENCE] Signature: Callee packet accesses object members through r0, compares r1 as a selector and locks the object mutex. */

extern void func_001d32e4(void *object, int index);

void func_001d32e0(void *object, int index)
{
    func_001d32e4(object, index);
}
