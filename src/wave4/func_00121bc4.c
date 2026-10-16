/* [INFERENCE] Signature: Callee packet preserves r0 and forwards it as the second argument to func_00186224/func_0012edd4. */

extern void func_00121bc8(void *object);  /* intrusive-list object; callee rewrites its links */

void func_00121bc4(void *object)
{
    func_00121bc8(object);
}
