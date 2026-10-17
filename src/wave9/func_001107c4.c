/* [INFERENCE] Callback result is unused; the argument is read through one pointer. */
void func_001107c4(void (*callback)(void *), void *const *argument)
{
    callback(*argument);
}
