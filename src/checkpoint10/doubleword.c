struct ThreeWords { unsigned long long first; unsigned third; };
void *func_001c4b10(void *storage)
{
    struct ThreeWords *words = storage;
    words->first = 0;
    words->third = 0;
    return words;
}
