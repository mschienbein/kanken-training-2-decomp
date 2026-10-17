#include "game/layout_draw.h"
#include <new>
#include "game/functions.h"

union StaticGuardW22E {
    unsigned int word;
    unsigned char initialized;
};

/* [INFERENCE] These singleton vtables process text tags: their methods handle
 * tab/newline tags and update the text writer's position and bounds. */
struct TextTagProcessorW22E {
    const void *const *vtable;
    TextTagProcessorW22E(const void *const *table) : vtable(table) {}
};

/* INTEGRATOR: move to include/game/text_tag_processor.h */
extern "C" {
extern volatile StaticGuardW22E data_002438d0;
extern volatile StaticGuardW22E data_002438d8;
extern TextTagProcessorW22E data_002438d4;

extern const void *const data_00224c90[];
extern const void *const data_00224ca8[];
extern void func_00216ba4(unsigned int category, unsigned int code);
extern void func_00216e4c(void);
extern void func_00100c94(volatile unsigned int *guard);
}

extern "C" void func_00214e40(void)
{
    if (!(data_002438d0.initialized & 1) && func_00191c80(&data_002438d0.word)) {
        TextTagProcessorW22E *processor = new (&data_002438d4) TextTagProcessorW22E(data_00224c90);
        /* [INFERENCE] callee identity: destructor registration was inlined by armlink. */
        func_00216140(processor, (ObjectDestructor)func_00216ba4, func_00100000);
        /* [INFERENCE] callee identity: guard release was inlined by armlink. */
        func_00215ec4(&data_002438d0.word);
    }
    if (!(data_002438d8.initialized & 1) && func_00191c80(&data_002438d8.word)) {
        TextTagProcessorW22E *processor = new (&data_002438dc) TextTagProcessorW22E(data_00224ca8);
        /* [INFERENCE] callee identity: destructor registration was inlined by armlink. */
        func_00216140(processor, (ObjectDestructor)func_00216e4c, func_00100000);
        func_00100c94(&data_002438d8.word);
    }
}
