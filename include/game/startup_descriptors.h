#ifndef GAME_STARTUP_DESCRIPTORS_H
#define GAME_STARTUP_DESCRIPTORS_H

struct ItemPageSet;
struct NameTable;
struct StartupItemPageSet;
struct StartupLink;

#include "game/item_pages.h"
#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
#ifdef __cplusplus
struct StartupLink {
    int category;
    int identifier;
    int destination;
    StartupLink(int category_, int identifier_)
        : category(category_), identifier(identifier_), destination(-1) {}
};
#endif

#ifdef __cplusplus
struct StartupItemPageSet : ItemPageSet {
    unsigned char reserved18[4];
    StartupItemPageSet();
};
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern struct NameTable *bss_00246d3c[7];
extern struct NameTable *bss_00246d58[7];
extern struct StartupLink *bss_00246d74[17];
extern struct StartupLink *bss_00246db8[17];
extern struct StartupLink *bss_00246dfc[12];
extern struct StartupLink *bss_00246e2c[33];
extern void func_00210964(void);
#ifdef __cplusplus
}
#endif

#endif
