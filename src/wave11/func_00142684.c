#include "game/service_protocol.h"
#include <string.h>
unsigned func_00142684(const char *text)
{
    unsigned size = text ? strlen(text) + 1 : 0;
    return func_001426a8(text, size);
}
