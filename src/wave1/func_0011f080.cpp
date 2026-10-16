#include "game/runtime.h"

class ResourceObject {
public:
    virtual ~ResourceObject();
    virtual void reserved02();
    virtual void reserved03();
    virtual void reserved04();
    virtual void reserved05();
    virtual void reserved06();
    virtual void reserved07();
    virtual void reserved08();
    virtual void reserved09();
    virtual void reserved10();
    virtual void reserved11();
    virtual void reserved12();
    virtual void reserved13();
    virtual void reserved14();
    virtual void reserved15();
    virtual void reserved16();
    virtual void reserved17();
    virtual void reserved18();
    virtual void reserved19();
    virtual void reserved20();
    virtual void reserved21();
    virtual void reserved22();
    virtual void reserved23();
    virtual void reserved24();
    virtual void reserved25();
    virtual void reserved26();
    virtual void reserved27();
    virtual void reserved28();
    virtual void release();
};

struct ResourceEntry {
    int active;
    void *allocation;
    ResourceObject *object;
    unsigned char reserved[3];
    unsigned char flag;
    unsigned char tail[8];
};

struct ResourceTable {
    ResourceEntry *entries;
    int count;
    int activeCount;
};

extern "C" ResourceTable bss_002d3d60;

extern "C" void func_0011f080(ResourceEntry *entry)
{
    if (entry->active) {
        entry->active = 0;
        entry->flag = 0;
        if (entry->allocation) {
            func_001852f4(func_0013714c(entry->allocation));
            entry->allocation = 0;
        }
        if (entry->object) {
            entry->object->release();
            delete entry->object;
            entry->object = 0;
        }
        --bss_002d3d60.activeCount;
    }
}
