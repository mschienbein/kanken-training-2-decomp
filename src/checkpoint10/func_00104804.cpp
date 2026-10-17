#include "game/graphics_resources.h"
#include "game/runtime.h"

class CleanupElement;

class CleanupOwner {
public:
    virtual void reserved00();
    virtual void reserved01();
    virtual void reserved02();
    virtual void release(CleanupElement *element);
};

class CleanupElement {
public:
    virtual void destroy();
    CleanupOwner *owner;
};

struct ResourceResetState {
    BufferState buffer;
    unsigned reserved0c;
    CleanupElement *element10;
    CleanupElement *element14;
    CleanupElement *element18;
    CleanupElement *element1c;
    CleanupElement *element20;
    CleanupElement *element24;
    CleanupElement *element28;
    CleanupElement *element2c;
    CleanupElement *element30;
};

/* INTEGRATOR: move to include/game/resource_reset.h */
extern "C" {
/* INTEGRATOR: differs from src/wave1/func_00135a58.c: reset also owns object slots at +0x10 through +0x30. */

}

static inline void release_element(CleanupElement **slot)
{
    if (*slot) {
        CleanupElement *element = *slot;
        element->destroy();
        element->owner->release(element);
        *slot = 0;
    }
}

extern "C" void func_00104804(void)
{
    if (bss_0049da58.buffer.buffer) {
        func_0019dc20(bss_0049da58.buffer.buffer);
        bss_0049da58.buffer.buffer = 0;
        bss_0049da58.buffer.count = 0;
        bss_0049da58.buffer.capacity = 0;
    }
    release_element(&bss_0049da58.element10);
    release_element(&bss_0049da58.element14);
    /* ORIGINAL BUG: element24 is released here instead of element18, then tested again below. */
    release_element(&bss_0049da58.element24);
    release_element(&bss_0049da58.element1c);
    release_element(&bss_0049da58.element20);
    release_element(&bss_0049da58.element24);
    release_element(&bss_0049da58.element28);
    release_element(&bss_0049da58.element2c);
    release_element(&bss_0049da58.element30);
}
