#ifndef GAME_SELECTION_H
#define GAME_SELECTION_H

#include <stdint.h>

/* Selection state at 0x0028c1f0; accessed selections are signed halfwords. */
struct SelectionState {
    unsigned char reserved00[0x4c];
    short selection4c;
    unsigned char reserved4e[4];
    short selection;
    short mode;
};

struct W6ListItem {
    unsigned char reserved00[0x12];
    short value;
};

struct W6ListGroup {
    unsigned int reserved00;
    int count;
    struct W6ListItem **items;
    unsigned char reserved0c[0x6c];
    int mode;
    int selected;
    int preserve_selected;
};

struct W6ListController {
    unsigned reserved00[8]; /* retain four-byte prefix alignment in C and C++ */
    struct W6ListGroup *lists[21];
    unsigned char dirty;
};

struct W6CategoryItem {
    int kind;
    unsigned int reserved04[2];
};

struct W6Category {
    int reserved00;
    short count;
    unsigned char reserved06[0x5e];
    struct W6CategoryItem items[12];
};

struct W6Selector;
struct W6SelectionScreen {
    unsigned char reserved00[0x4c];
    struct W6ListController *list;
    unsigned char reserved50[8];
    struct W6Selector *selector;
    unsigned char reserved5c[0x54];
    int selection;
};

struct GlyphRecord {
    unsigned reserved00[7];
    unsigned char reserved1c;
    signed char kind;
    unsigned char reserved1e[6];
};

struct RecordSet {
    unsigned char reserved00[8];
    const struct GlyphRecord *records;
};

struct MetadataHeader {
    short year;
    short identifier;
    short width;
    short height;
};

struct MetadataEntry {
    const struct MetadataHeader *header;
    uintptr_t resource;
    int index;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct SelectionState bss_0028c1f0;
extern struct W6Category bss_00290120[];
extern const struct MetadataEntry data_0023fce4[];
extern int func_001678c0(const struct RecordSet *set, int index);
extern int func_001698f4(int identifier, int year);
extern int func_00169920(int group, int index, int value);
extern int func_001718ec(struct W6SelectionScreen *screen);
extern int func_00171c7c(struct W6SelectionScreen *screen);
extern int func_00171c58(int category, int item);
extern int func_00171d24(int category);
extern int func_00174160(const struct W6ListController *controller, int list, int item);
extern void func_00174210(struct W6Selector *selector, int resource);
extern void func_001780c4(struct W6ListController *controller, int list, int item, int enabled);
extern int func_00178144(void);
extern void func_00178e98(struct W6ListController *controller, int index);
extern int func_00178f84(struct W6ListController *controller, int index);
extern int func_00179148(void);
extern int func_001795bc(void);
extern void func_001e8934(struct W6ListController *controller, int index);
#ifdef __cplusplus
}
#endif

#endif
