#ifndef GAME_RESOURCE_TABLES_H
#define GAME_RESOURCE_TABLES_H

struct FontResource;
struct ResourceHandle;


#include <stdint.h>
#include "game/runtime.h"
#include "game/font.h"

struct ResourceObject;
struct ResourceTableEntry {
    int active;
    void *allocation;
    struct ResourceObject *object;
    unsigned char reserved[3];
    unsigned char flag;
    unsigned char tail[8];
};

struct ResourceTable {
    struct ResourceTableEntry *entries;
    int count;
    int activeCount;
    unsigned char reserved0c[0x28];
    struct FontResource *font;
    float width, height;
    unsigned char mode, flag41, flag42, flag43, flag44, flag45;
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
};;

struct Entry {
    int id;
    struct ResourceHandle *resource;
};
struct EntryList {
    struct Entry *entries;
    int count;
};
struct DetailEntry {
    int id;
    int value;
    int flags;
    int length;
    struct ResourceHandle *resource;
};
struct DetailList {
    struct DetailEntry *entries;
    int count;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct ResourceTable bss_002d3d60;
extern struct EntryList data_0024225c;
extern struct DetailList data_00242264;
extern unsigned *data_0023fd80;
extern unsigned char *func_001971a0(uintptr_t resource, int index);
#ifdef __cplusplus
}
#endif




#endif
