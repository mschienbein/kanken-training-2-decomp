#include <stddef.h>

struct ArrayElement {
    unsigned char reserved00[0xd4];
};

struct ElementArray {
    unsigned reserved00;
    struct ArrayElement elements[8];
};

extern void func_001c0e44(void *element);
extern void *__aeabi_vec_ctor_nocookie_nodtor(void *storage, void (*constructor)(void *),
                                           size_t element_size, size_t element_count);

struct ElementArray *func_001ef960(struct ElementArray *array)
{
    unsigned *elements = __aeabi_vec_ctor_nocookie_nodtor(array->elements, func_001c0e44,
                                                        sizeof(struct ArrayElement), 8);
    return (struct ElementArray *)(elements - 1);
}
