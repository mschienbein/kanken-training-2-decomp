#ifndef GAME_SELECTION_H
#define GAME_SELECTION_H

struct CategoryPattern;
struct Screen;
struct SelectionCallbacks;
struct SelectionInterface;
struct SelectionHintPanel;
struct W8DisplaySet;

#include "game/category_pattern.h"

#include <stdint.h>

/* Selection state at 0x0028c1f0; accessed selections are signed halfwords. */
struct SelectionState {
    unsigned char reserved00[0x4c];
    short selection4c;
    unsigned char reserved4e[4];
    short selection;
    short mode;
    unsigned char reserved56[2];
    short attempts;
    short successes;
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
    struct W6ListGroup *lists[20];
    unsigned char reserved70[4];
    unsigned char refreshing;
};

struct W6CategoryItem { int kind; short values[2][2]; };

struct W6Category {
    short identifier, group_count, count, value;
    struct CategoryPattern patterns[9];
    unsigned char reserved62[2];
    struct W6CategoryItem items[12];
};

struct W6Selector;
struct W6HandlePanel10;
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

/* The pointer vector is variable-sized; only its +0x20 base is established. */
struct W7dMenuControl {
    unsigned char reserved00[0x20];
#ifndef __cplusplus
    unsigned *flags[];
#endif
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
extern void func_00179068(struct W7dMenuControl *control, int index, int enabled);
extern int func_00179148(void);
extern int func_001795bc(void);
extern void func_001e8934(struct W6ListController *controller, int index);
extern void func_001e9944(struct W6ListController *controller);
extern void func_001e9950(struct W6ListController *controller);
extern void func_00179600(struct W6ListController *controller, int index, int active);
extern void func_001e9818(void *screen, int index, int first, int second);
extern void func_00173d8c(struct W6HandlePanel10 *panel, int resource);
#ifdef __cplusplus
}
#endif


struct SelectionCallbacks {
    const void *reserved00[6];
    void (*begin)(struct W6SelectionScreen *screen);
    const void *reserved1c;
    void (*end)(struct W6SelectionScreen *screen);
};

struct SelectionInterface {
    const struct SelectionCallbacks *vtable;
};

#ifdef __cplusplus
extern "C" {
#endif
void func_00171a98(struct W6SelectionScreen *screen, int use_active);
void func_001a78a4(struct W6SelectionScreen *screen, int use_active);
extern void func_00181b58(unsigned short screen);
extern void func_001a73e8(struct W6SelectionScreen *screen);
extern void func_001ab4e4(struct W6SelectionScreen *screen);
extern int func_001674e4(int category, int kind, int mode);
extern int func_00167590(int category, int member);
extern int func_001dd44c(int category, int kind, int mode);
extern int func_001dd61c(int category);
extern void func_00190dc0(int category);
extern int func_00171bf0(int category, int kind);
extern int func_00173350(int sound);
extern void func_00173408(struct SelectionHintPanel *panel, int state);
extern int func_00173608(void);
extern void func_00173630(struct W6ListController *controller, int list, int resource);
extern void func_0017365c(struct W6ListController *controller, int list, int resource,
                          unsigned preserve, int source_list, int select);
extern void func_00173e48(struct SelectionHintPanel *panel, int active);
extern void func_00173ef4(struct Screen *screen);
extern void func_00173fb4(struct Screen *screen);
extern int func_00174444(void);
extern int func_00174470(void);
extern void func_0017a034(struct Screen *screen, int index, int value);
extern void func_001a2a78(struct W6SelectionScreen *screen, int from_list);
extern void func_001a2e6c(struct W6SelectionScreen *screen, int from_list);
extern void func_001b472c(void *screen, int index);
extern int func_001dc150(void);
extern int func_001dc160(void);
#ifdef __cplusplus
}
#endif

#endif
