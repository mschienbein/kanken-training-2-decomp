struct ZeroPair { unsigned long long value; };

void *func_00112fa8(void *object)
{
    struct ZeroPair *pair = object;
    pair->value = 0;
    return object;
}
