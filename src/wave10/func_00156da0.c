struct ResetReference {
    unsigned value;
};

/* No direct callers establish a result; only the writable pointer is evidenced. */
void func_00156da0(struct ResetReference *reference)
{
    reference->value = 0;
}
