/* [INFERENCE] Pointer-only callback adapter; no caller fixes the payload type. */
void func_0011e98c(void (*callback)(void *), void *const *argument)
{
    callback(*argument);
}
