#ifndef GAME_MATERIAL_H
#define GAME_MATERIAL_H

struct DisplayObject;
struct PaneColor;
struct ResourceArray;
struct ResourceDimensions;

#include "game/runtime.h"
#include "game/screen.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */




struct ResourceArray {
    void *buffer;
    unsigned int count;
    unsigned int capacity;
    unsigned int state;
};

/* The same 28-byte record is queried for metadata and releases its first handle. */
struct ResourceRecord {
    unsigned present;
    unsigned reserved04[2];
    unsigned info[3];
    int format;
};

struct ResourceDimensions {
    void *resource;
    int width;
    int height;
    unsigned char reserved0c[0x10];
};

struct PaneColor {
    unsigned char red, green, blue, alpha;
};




#ifdef __cplusplus
#include <preinclude.h>
#include "nw/lyt/lyt_Picture.h"
extern "C" {
void func_0017aa90(nw::lyt::Pane *pane, unsigned char red, unsigned char green,
                   unsigned char blue, unsigned char alpha);
void func_0014c5c4(nw::lyt::Picture *pane, nw::lyt::Material **material, int resource, int replace);
nw::lyt::BlendMode *func_0014d1ec(nw::lyt::Material *material);
nw::lyt::Material *func_0017fe04(nw::lyt::Material *material);
void func_0017fc7c(nw::lyt::Material *material, unsigned char count);
#endif
extern struct ResourceArray bss_002d5ea0;
extern unsigned char *data_00242fc8;
extern const unsigned data_00243148[0x5c];
extern void func_0018e764(void *destination, const void *source, unsigned size);
extern void func_00181100(struct DisplayObject *object, int resource, int replace);
extern void func_0014aaf4(struct DisplayObject *object, int mode, float value);
extern void func_0014ab30(struct DisplayObject *object, unsigned int value);
extern void func_0015e9dc(struct DisplayObject *object, int index,
                   unsigned char alpha);
extern int func_0017e948(int index);
extern int func_0017e970(int index);
extern void func_00180404(struct DisplayObject *object, int index);
extern void func_00130f04(void);
extern void func_00182744(const void *source, void *destination, unsigned size);
#ifdef __cplusplus
}
#endif

#endif
