#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include "game/display_object.h"

struct PairStorage;
struct W10DrawingInput;


/* Menu screen with widget slots (partial). Offsets are fixed by the original code. */

#ifdef __cplusplus
extern "C" {
#endif

struct ResourceEntry;





struct DisplayObjectList {
    unsigned char reserved00[0x2c];
    struct DisplayObject *head;
};

/* D04: static registrations initialize the complete owner at +4, stride 0xd4. */
struct ResourceRegistrationSlot {
    void *resource;
    struct DisplayObject display;
    unsigned char reservedd0[4];
};
struct ResourceRegistrationGroup {
    unsigned reserved;
    struct ResourceRegistrationSlot entries[8];
};
struct ResourceRegistrationState {
    unsigned initialized;
    unsigned char reserved04[8];
    unsigned first, second, third;
    struct ResourceRegistrationGroup groups[2];
    unsigned char active, pending;
    unsigned short reservedd62;
    unsigned count;
};
struct MenuOptionGroup;
struct Screen;
struct ScreenInitializationState;
struct W8DisplaySet;
struct WidgetActionState {
    unsigned char reserved00[0xbc];
    unsigned active;
};
struct DisplayQueueFlags {
    unsigned char reserved00[2];
    unsigned char enabled;
    unsigned char available;
    signed char active;
    unsigned char reserved05[3];
    struct DisplayObject *object;
    int index;
    unsigned pending;
    void (*callback)(void);
    unsigned char reserved18[0x0c];
    void (*callbacks[21])(void);
};
struct DisplayQueueCounter {
    unsigned ready;
    unsigned count;
    unsigned enabled;
    int mode;
    unsigned char active;
    unsigned char reserved11[3];
    int counter;
    unsigned capacity;
};


struct DisplayHandle {
    struct DisplayObject *object;
    unsigned char reserved04[0x14];
    float x;
    float y;
#ifdef __cplusplus
    void set_active(int active);
#endif
};

/* [INFERENCE] The native two-float constructor and the guarded 32-slot owner
 * establish the complete display-handle allocation, including its text tail. */
struct MotionPair {
    float x, y;
#ifdef __cplusplus
    MotionPair();
#endif
};

struct W8DisplayHandle {
    struct DisplayObject *object;
    struct MotionPair position;
    struct MotionPair scale;
    unsigned char color[4];
    unsigned char text[0x80];
#ifdef __cplusplus
    W8DisplayHandle()
    {
        object = 0;
        position.x = 0.0f;
        position.y = 0.0f;
        scale.x = 1.0f;
        scale.y = 1.0f;
        color[0] = 255;
        color[1] = 255;
        color[2] = 255;
        color[3] = 255;
    }
#endif
};

struct W8DisplaySet {
    unsigned char reserved00[0x1300];
#ifndef __cplusplus
    struct W8DisplayHandle *handles[];
#endif
};

#ifdef __cplusplus
/* The style singleton owns exactly 32 handles after the shared display prefix;
 * other W8DisplaySet clients still use only their evidenced flexible prefix. */
struct StyleOwner {
    W8DisplaySet base;
    W8DisplayHandle *slots[32];
};
#endif

struct AnimationContext {
    unsigned int reserved00;
    const struct NameTable *table;
};


/* [INFERENCE] Value object passed in s0/s1 by the container's position setter. */
struct WidgetPosition {
    float x;
    float y;
};
/* Named entries selected by index; `kind` 1 means `names` is valid. */
struct NameTableVTable;
struct NameTable {
    const struct NameTableVTable *vtable;
    int kind;                               /* 0x04 */
    int category;                           /* 0x08 */
    int identifier;                         /* 0x0c */
    const char *name;                       /* 0x10 */
    int count;                              /* 0x14 */
    const char **names;                     /* 0x18 */
#ifdef __cplusplus
    NameTable();
    NameTable(int category, int identifier, const char *name, int count, ...);
#endif
};

struct WidgetSlotLabel {
    short label_index;
    unsigned char enabled;
};

union WidgetSlotState {
    unsigned int flags;
    struct WidgetSlotLabel label;
    unsigned char byte_flags[4];
};

struct WidgetSlot {
    struct DisplayObject *object;
    union WidgetSlotState state;
    unsigned short text[2];
    unsigned short output[16];
    unsigned short *label;
    unsigned short *formatted;
#ifdef __cplusplus
    void set_active(int enabled);
#endif
};

/* Shared fixed prefix of widget controllers. The following pointer-vector has
 * a different extent in menu screens and character panels. */
struct WidgetController {
    unsigned char reserved00[0x0c];
};

static __forceinline struct WidgetSlot **widget_slots(struct WidgetController *controller)
{
    return (struct WidgetSlot **)(controller + 1);
}

struct Screen {
    struct WidgetController controller;
    struct WidgetSlot *slots[0x36];         /* 0x00c; highest index used: 0x35 */
    unsigned char reservede4[4];
    int selected;                           /* 0x0e8 */
    int count;                              /* 0x0ec */
    int mode;                               /* 0x0f0 */
    unsigned char reservedf4[0xf0];
    const struct NameTable *const *labels;  /* 0x1e4 */
};

extern struct ResourceRegistrationState bss_0049cbd0;
extern struct DisplayQueueFlags data_0023ee9c;
extern struct DisplayQueueCounter bss_00495c70;
extern int func_0019efd0(void);
extern void func_001b6c90(struct MenuOptionGroup *options);
extern void func_001b7e9c(struct MenuOptionGroup *options);
extern void func_00176b24(struct Screen *selection);
extern void func_0017697c(struct Screen *selection);
extern void func_0015eac8(struct DisplayObject *object, int index, float value);
extern void func_001701b4(struct WidgetController *controller, int index, int enabled);
extern void func_00175b8c(void *screen);
extern void func_00179160(struct W8DisplaySet *set, int index, float x, float y);
extern void func_00166eb0(struct W8DisplaySet *set, int index, float x, float y);
extern unsigned data_0023ecdc;
extern struct StyleOwner bss_00245364;
extern struct StyleOwner *func_001eb0d0(struct StyleOwner *owner);
extern struct StyleOwner *func_001eade0(void);
extern struct DisplayObject *func_0017a818(const struct NameTable *table,
    int screen, int active, float x, float y, float z);
extern void func_00175ab8(struct W8DisplaySet *screen, int index,
                         int red, int green, int blue, int alpha);
extern void func_00166eac(struct W8DisplaySet *screen, int index, float x, float y);
extern void func_00175848(void);
extern void func_001e807c(struct ScreenInitializationState *state);
extern void func_001adabc(struct WidgetActionState *state);
extern void func_00196230(int screen, int index);
extern void func_00196144(int screen, int index);
extern void func_00176d98(void);
extern void func_00177084(void);
extern void func_00176df4(void);
extern void func_00181448(struct DisplayObject *object);
extern void func_0019637c(struct DisplayObject *object,
                          struct DisplayObject **head, int primary);
extern const struct NameTable *bss_00246f4c[];
extern void func_0016e7e0(struct WidgetController *controller, int index, int enabled);
extern void func_0016e3e0(struct WidgetController *controller, int index, int value);
extern void func_0016e648(struct WidgetController *controller, int index, int character);
extern struct AnimationContext bss_00246b48;
extern void func_0015e948(struct DisplayObject *object, int active);
extern void func_0015e60c(const struct ResourceEntry *entry, float *width, float *height);
extern void func_0015e78c(struct ResourceEntry *entry, unsigned value);
extern void func_0015e830(struct ResourceEntry *entry, unsigned value);
extern void func_0015e5c4(struct DisplayObject *object, int index, float x, float y, float z);
extern void func_0016f534(struct DisplayObject *object, int index, float duration);
extern void func_0016e300(struct WidgetController *controller, int slot_index,
                          int red, int green, int blue, int alpha);
extern void func_00170030(struct DisplayObject *object, int red, int green, int blue, int alpha);
extern void func_00170088(struct DisplayObject *object, int red, int green, int blue, int alpha);
extern void func_00167e08(struct DisplayObject *object, int vertex_index,
                          int red, int green, int blue, int alpha);
extern void func_0016fff8(struct DisplayObject *object, int red, int green, int blue, int alpha);
extern void func_00170014(struct DisplayObject *object, int red, int green, int blue, int alpha);
extern void func_0015e954(struct ResourceEntry *link, int active);
extern void func_0015e4a8(struct ResourceEntry *surface, float character_spacing, float line_spacing);
extern void func_0015e634(struct ResourceEntry *surface, float width, float height);
extern void func_0015e5d4(struct ResourceEntry *surface, float x, float y, float z);
extern void func_0015e3f4(struct ResourceEntry *surface, int red, int green, int blue, int alpha);
extern void func_0015e534(struct ResourceEntry *surface, int red, int green, int blue, int alpha);
extern void func_0015e57c(struct ResourceEntry *surface, int red, int green, int blue, int alpha);
extern void func_00180358(struct DisplayObject *object, int first, int second);
extern void func_0015e684(struct DisplayObject *object, int index, float x, float y, float z);
extern void func_0018271c(struct DisplayObject *object, unsigned char alpha);
extern void func_00181700(struct DisplayObject *object, float x, float y, float z);
extern void func_00181734(struct DisplayObject *object, float width, float height);
extern void func_001826fc(struct DisplayObject *object, int active);
extern void func_00195a08(struct DisplayObject *object, int index, int active);
extern void func_0016de90(struct Screen *screen, int index);
extern void func_0016e1d0(struct Screen *screen, int index, int enabled);
extern unsigned char func_0017a9a0(const struct DisplayObject *object, int index);
extern void func_0017a968(struct DisplayObject *object, const struct NameTable *table, int index, int mode);
extern void func_001b7c9c(struct Screen *screen, int index, int value, const struct NameTable *const *labels);
extern void func_0017a9ac(struct Screen *screen);
extern void func_00170e50(struct Screen *screen, int index);
extern void func_0017a6f4(struct Screen *screen, int index);
extern void func_001795cc(struct Screen *screen, int first, int second);
extern void func_0017a0e0(struct W8DisplaySet *set, int index, int active);
extern void func_001816e0(struct DisplayObject *object, float x, float y);
extern void func_0015e474(struct DisplayObject *object, int index, float x, float y, float z);
extern void func_0015e684(struct DisplayObject *object, int index, float x, float y, float z);
extern void func_001963f8(struct DisplayObject *object);
extern struct DisplayObject *func_00145b5c(int screen, int index);
extern struct DisplayObject *data_00242248;
extern struct DisplayObject *data_0024224c;
extern struct DisplayObject *data_00242250;
extern void func_0018695c(struct DisplayObject *object,
                          struct DisplayObject **head, int primary);
extern void func_001eafa0(void *screen, int index, const unsigned short *text);
extern void func_0016de90(struct Screen *screen, int index);

#ifdef __cplusplus
}
#endif


struct W10DrawingInput {
    unsigned char reserved00[4];
    int identifiers[2];
    struct DisplayObject *displays[3];
    unsigned char notification_active, reserved19;
    unsigned short characters[0x440];
    short field89a;
    short count;
    unsigned char reserved89e[0x1a];
    unsigned char active;
    unsigned char finished;
    unsigned char reserved8ba[2];
    float scale;
    float progress;
    int state;
    unsigned char reserved8c8[0x14];
    unsigned char buffer[0x800];
    unsigned int reserved10dc;
    int status;
};


#ifdef __cplusplus
extern "C" {
#endif
extern void func_0015e698(struct ResourceEntry *link, float x, float y, float z);
extern void func_0019cecc(void *destination, unsigned int size);
extern void func_00180344(void);
extern void func_00166d5c(struct W10DrawingInput *input, int active, float x, float y);
extern struct NameTable *bss_00245344[8];
extern void func_00202ca8(void);
extern void func_0016dbc4(struct W10DrawingInput *input);
extern void func_0016dc94(struct W10DrawingInput *input, int index);
extern int func_00115138(void);
extern void func_00195900(struct DisplayObject *object, int index, const char *name, int mode);
#ifdef __cplusplus
}
#endif

#endif
