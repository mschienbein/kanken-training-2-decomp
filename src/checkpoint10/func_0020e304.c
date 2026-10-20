#include <string.h>

/* Compiler-emitted startup initialization of the first word at data_002438a4.
 * func_0012159c accesses this service state before publishing a handle.
 * [INFERENCE] The original enclosing type and field name are unrecovered. */
extern unsigned data_002438a4;

void func_0020e304(void)
{
    memset(&data_002438a4, 0, sizeof(data_002438a4));
}
