#include "game/exp_heap.h"
#include "game/formatted_text.h"
#include <stdarg.h>




void func_0015dc14(TextFormatContainer *container, const unsigned short *format, ...)
{
    va_list arguments;
    unsigned short capacity = func_001f15e0(container->text);
    unsigned short *buffer = static_cast<unsigned short *>(func_0019ffa8(capacity * 2));
    va_start(arguments, format);
    func_0010083c(buffer, capacity, format, arguments);
    container->text->set_text(buffer, 0);
    func_001852f8(buffer);
    va_end(arguments);
}
