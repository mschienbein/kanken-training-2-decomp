struct ZeroWord { unsigned value; };

void *func_00186488(void *object)
{
    struct ZeroWord *word = object;
    word->value = 0;
    return object;
}
