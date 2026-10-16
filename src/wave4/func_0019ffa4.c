/* [INFERENCE] Signature: Callee packet forwards incoming size to func_00195104 with fixed alignment 4/flags 0; numerous callers dereference the returned allocation. */

extern void * func_0019ffa8(unsigned size);

void * func_0019ffa4(unsigned size)
{
    return func_0019ffa8(size);
}
