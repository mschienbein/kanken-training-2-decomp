/* [INFERENCE] No direct callers identify the callback payload beyond one pointer. */
void func_001415a4(void (*callback)(void *), void *const *payload)
{
    callback(*payload);
}
