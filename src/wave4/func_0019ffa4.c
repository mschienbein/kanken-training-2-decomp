/* Callers supply size in r0 and an extra pointer in r1: usually data_002232d5,
 * but func_00193110 supplies a stack address. Neither entry writes through r1;
 * func_0019ffa8 consumes only size. [INFERENCE] Unused const tag name/type. */

extern void * func_0019ffa8(unsigned size);

void * func_0019ffa4(unsigned size, const void *tag)
{
    return func_0019ffa8(size);
}
