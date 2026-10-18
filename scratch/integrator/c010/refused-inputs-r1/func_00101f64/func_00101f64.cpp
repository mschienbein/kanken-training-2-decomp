#include <stdint.h>
#include "game/runtime.h"
#include "game/font.h"

/* [INFERENCE] Owner/type and method spellings are semantic names; only the
 * object pointer and the second virtual slot are observed by this teardown. */
class FontTableOwner {
public:
    virtual void slot0();
    virtual void destroy();
};
struct FontVTable {
    void (*slot0)(FontResource *font);
    void (*destroy)(FontResource *font);
};
struct ResourceTableEntry;
/* INTEGRATOR: differs from include/game/resource_tables.h: teardown reveals
 * the owner and two allocations within reserved0c, plus the trailing font pool. */
struct ResourceTableInternal {
    ResourceTableEntry *entries;
    int count, activeCount;
    unsigned reserved0c;
    FontTableOwner *owner;
    void *firstAllocation;
    unsigned reserved18;
    void *secondAllocation;
    unsigned char reserved20[0x14];
    FontResource *font;
    float width, height;
    unsigned char style[4];
    unsigned char enabled, option;
    unsigned char reserved46[6];
    void *defaultSheets;
    unsigned char firstEnabled;
    unsigned char reserved51[3];
    void *firstSheets;
    uintptr_t firstResource;
    int firstIndex;
    unsigned char secondEnabled;
    unsigned char reserved61[3];
    void *secondSheets;
    uintptr_t secondResource;
    int secondIndex;
};
/* INTEGRATOR: move to include/game/resource_tables.h */
extern "C" {
/* INTEGRATOR: differs from include/game/resource_tables.h: see expanded layout above. */
extern ResourceTableInternal bss_002d3d60;
unsigned char *func_0019dc04(void);
void func_0019d95c(uintptr_t resource, int index, int mode);
}

extern "C" void func_00101f64(void)
{
    if (bss_002d3d60.defaultSheets)
        func_0019dc20(bss_002d3d60.defaultSheets);
    if (bss_002d3d60.secondAllocation)
        func_0019dc20(bss_002d3d60.secondAllocation);
    if (bss_002d3d60.firstAllocation)
        func_0019dc20(bss_002d3d60.firstAllocation);
    if (bss_002d3d60.owner)
        bss_002d3d60.owner->destroy();
    unsigned char *file = func_0019dc04();
    void *sheets = bss_002d3d60.defaultSheets;
    if (file) {
        func_0019dbec(bss_002d3d60.font);
        func_0019da14(bss_002d3d60.font, reinterpret_cast<FontFile *>(file));
        func_0019d9d8(bss_002d3d60.font, sheets);
    }
    if (bss_002d3d60.firstEnabled) {
        bss_002d3d60.firstEnabled = 0;
        func_0019d95c(bss_002d3d60.firstResource, bss_002d3d60.firstIndex, 0);
        if (bss_002d3d60.firstSheets) {
            func_0019dc20(bss_002d3d60.firstSheets);
            bss_002d3d60.firstSheets = 0;
        }
    }
    if (bss_002d3d60.secondEnabled) {
        bss_002d3d60.secondEnabled = 0;
        func_0019d95c(bss_002d3d60.secondResource, bss_002d3d60.secondIndex, 0);
        if (bss_002d3d60.secondSheets) {
            func_0019dc20(bss_002d3d60.secondSheets);
            bss_002d3d60.secondSheets = 0;
        }
    }
    if (bss_002d3d60.font) {
        func_0019dbec(bss_002d3d60.font);
        if (bss_002d3d60.font)
            bss_002d3d60.font->vtable->destroy(bss_002d3d60.font);
    }
}
