#ifndef GAME_MATERIAL_H
#define GAME_MATERIAL_H

struct BoundDisplayObject;
struct ColoredDisplayBinding;
struct DisplayPaneHolder;
struct DisplayPane;
struct Material;
struct MaterialPair;
struct MaterialVTable;
struct MaterialWord;
struct PaneColor;
struct ResourceArray;
struct ResourceDimensions;
struct TextureDisplayBinding;

#include "game/runtime.h"
#include "game/screen.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct MaterialPair {
    unsigned int mode : 8;
    unsigned int reserved : 24;
    float value;
};

struct MaterialWord { unsigned int value; };

struct Material {
    const struct MaterialVTable *vtable;
    unsigned char reserved04[0x28];
    unsigned int capacity;
    unsigned int count;
    unsigned char *parameters;
    unsigned char reserved38[0x15];
    unsigned char flags;
};

struct DisplayPaneHolder {
    unsigned char reserved00[0x10];
    struct DisplayPane *pane;
};

struct ColoredDisplayBinding {
    unsigned char reserved00[0x10];
    unsigned char alpha, red, green, blue;
    unsigned char reserved14[8];
    struct DisplayPaneHolder *holder;
};

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


struct MaterialVTable {
    void (*destroy)(struct Material *);
    void (*destroy_delete)(struct Material *);
};

struct TextureDisplayBinding {
    unsigned char reserved00[0x28];
    int mode;
    int resource;
    unsigned char reserved30[4];
    struct DisplayPane *pane;
    struct Material **material;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct Material *func_0017fd68(struct DisplayPane *pane);
extern struct MaterialPair *func_0014d254(struct Material *material);
extern struct MaterialWord *func_0014d1ec(struct Material *material);
extern void func_0017aa90(struct DisplayPane *pane, unsigned char red,
                        unsigned char green, unsigned char blue,
                        unsigned char alpha);
extern struct ResourceArray bss_002d5ea0;
struct Material *func_0017fe04(struct Material *material);
extern void func_0017fda4(struct DisplayPane *pane, struct Material *material);
void func_0017fd0c(struct Material *material, unsigned char count);
void func_0017fc7c(struct Material *material, unsigned char count);
void func_0014c5c4(struct DisplayPane *pane, struct Material **material,
                  int resource, int replace);
extern unsigned char *data_00242fc8;
extern const unsigned data_00243148[0x5c];
extern void func_0018e764(void *destination, const void *source, unsigned size);
extern void func_00181100(struct BoundDisplayObject *object, int resource, int replace);
extern void func_0014aaf4(struct BoundDisplayObject *object, int mode, float value);
extern void func_0014ab30(struct BoundDisplayObject *object, unsigned int value);
extern void func_0015e9dc(struct BoundDisplayObject *object, int index,
                   unsigned char alpha);
extern int func_0017e948(int index);
extern int func_0017e970(int index);
extern void func_00180404(struct BoundDisplayObject *object, int index);
extern void func_00130f04(void);
extern void func_00182744(const void *source, void *destination, unsigned size);
#ifdef __cplusplus
}
#endif

#endif
