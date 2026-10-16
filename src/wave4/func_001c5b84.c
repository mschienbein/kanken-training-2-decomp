/* [INFERENCE] Signature: Wrapper caller func_001c5b8c is a deleting-destructor pattern; the adjacent return-only callee preserves the object pointer. */

extern void * func_001c5b88(void *object);

void * func_001c5b84(void *object)
{
    return func_001c5b88(object);
}
