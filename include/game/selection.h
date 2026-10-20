#ifndef GAME_SELECTION_H
#define GAME_SELECTION_H
struct MenuScene;
struct SelectionMenuScene;
struct InitializationMenuScene;
#include "game/menu_scene.h"

struct CategoryPattern;
struct Screen;
struct SelectionHintPanel;
struct W8DisplaySet;

#include "game/category_pattern.h"
#include "game/w11_control.h"
#include "game/menu_controls.h"

#include <stdint.h>

/* Selection state at 0x0028c1f0; accessed selections are signed halfwords. */
struct SelectionState {
    unsigned char reserved00[0x4c];
    short selection4c;
    short previous;                     /* 0x4e; also queried as limit kind */
    short category;                     /* 0x50 */
    short selection;
    short mode;
    unsigned char reserved56[2];
    short attempts;
    short successes;
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
extern int func_001718ec(struct MenuScene *screen);
extern int func_00171c7c(struct MenuScene *screen);
extern int func_00171c58(int category, int item);
extern int func_00171d24(int category);
extern int func_00174160(const struct MenuControlSet *controller, int list, int item);
extern void func_00174210(struct W6Selector *selector, int resource);
extern void func_001780c4(struct MenuControlSet *controller, int list, int item, int enabled);
extern int func_00178144(void);
extern void func_00178e98(struct MenuControlSet *controller, int index);
extern int func_00178f84(struct MenuControlSet *controller, int index);
extern void func_00179068(struct MenuControlSet *control, int index, int enabled);
extern int func_00179148(void);
extern int func_001795bc(void);
extern void func_001e8934(struct MenuControlSet *controller, int index);
extern void func_001e9944(struct MenuControlSet *controller);
extern void func_001e9950(struct MenuControlSet *controller);
extern void func_00179600(struct MenuControlSet *controller, int index, int active);
extern void func_001e9818(void *screen, int index, int first, int second);
extern void func_00173d8c(struct W6HandlePanel10 *panel, int resource);
#ifdef __cplusplus
}
#endif




#ifdef __cplusplus
extern "C" {
#endif
void func_00171a98(struct SelectionMenuScene *screen, int use_active);
void func_001a78a4(struct InitializationMenuScene *screen, int use_active);
extern void func_00181b58(unsigned short screen);
extern int func_001674e4(int category, int kind, int mode);
extern int func_00167590(int category, int member);
extern int func_001dd44c(int category, int kind, int mode);
extern int func_001dd61c(int category);
extern void func_00190dc0(int category);
extern int func_00171bf0(int category, int kind);
extern void func_00173350(int sound);
extern void func_00173408(struct SelectionHintPanel *panel, int state);
extern int func_00173608(void);
extern void func_00173630(struct MenuControlSet *controller, int list, int resource);
extern void func_0017365c(struct MenuControlSet *controller, int list, int resource,
                          unsigned preserve, int source_list, int select);
extern void func_00173e48(struct SelectionHintPanel *panel, int active);
extern void func_00173ef4(struct Screen *screen);
extern void func_00173fb4(struct Screen *screen);
extern int func_00174444(void);
extern int func_00174470(void);
extern void func_0017a034(struct VisualLabelSet72 *owner, int index, int value);
extern void func_001a2a78(struct SelectionMenuScene *screen, int from_list);
extern void func_001a2e6c(struct SelectionMenuScene *screen, int from_list);
extern void func_001b472c(void *screen, int index);
extern int func_001dc150(void);
extern int func_001dc160(void);
#ifdef __cplusplus
}
#endif

#endif
