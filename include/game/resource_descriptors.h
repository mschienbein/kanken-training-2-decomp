#ifndef GAME_RESOURCE_DESCRIPTORS_H
#define GAME_RESOURCE_DESCRIPTORS_H

struct LayoutDescriptor;
struct ResourceDescriptor;
struct ResourceDispatch;

#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
#ifdef __cplusplus
class LayoutDescriptor {
public:
    int reserved00;
    int category;
    int resource;
    int options[4];
    float coordinates[6];

    LayoutDescriptor(int category, int resource, int option0, int option1,
                     int option2, int option3, float x0, float x1, float x2,
                     float x3, float x4, float x5);
};
#endif

#ifdef __cplusplus
class ResourceDescriptor {
public:
    const ResourceDispatch *dispatch;
    int state;
    int category;
    int resource;
    const char *name;

    ResourceDescriptor();
    ResourceDescriptor(int category, int resource, const char *name);
};
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern struct LayoutDescriptor *func_00164b14(struct LayoutDescriptor *object,
    int category, int resource, int option0, int option1, int option2, int option3,
    float x0, float x1, float x2, float x3, float x4, float x5);
extern struct LayoutDescriptor *bss_00246748[18];
extern struct ResourceDescriptor *bss_00246790[31];
extern struct ResourceDescriptor *bss_0024680c[31];
extern void func_0020e460(void);
#ifdef __cplusplus
}
#endif

#endif
