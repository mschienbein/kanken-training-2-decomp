/* The invoking array ABI returns the original element storage. */
void *func_001c5260(void *storage)
{
    unsigned *value = storage;
    *value = 0;
    return storage;
}
