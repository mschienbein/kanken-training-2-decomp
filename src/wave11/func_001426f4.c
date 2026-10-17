#include "game/service_protocol.h"
#include <string.h>
unsigned func_001426f4(const char *text)
{
    unsigned size = text ? strlen(text) + 1 : 0;
    return func_00142718(text, size);
}
