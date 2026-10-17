#include <string.h>

extern unsigned func_00142718(const char *text, unsigned size);

unsigned func_001426f4(const char *text)
{
    unsigned size = text ? strlen(text) + 1 : 0;
    return func_00142718(text, size);
}
