/* The hook is called by func_0013888c/func_00182ae4 with an object pointer in r0.
 * The adjacent callee tests r0 for null without dereferencing it.
 * [INFERENCE] Opaque writable object type/name; no caller consumes a result. */

extern void func_001c6a38(void *object);

void func_001c6a34(void *object)
{
    func_001c6a38(object);
}
