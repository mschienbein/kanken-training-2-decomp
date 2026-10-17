#include <string.h>

extern unsigned func_001426a8(const char *text, unsigned size);

unsigned func_00142684(const char *text)
{
    unsigned size = text ? strlen(text) + 1 : 0;
    return func_001426a8(text, size);
}
