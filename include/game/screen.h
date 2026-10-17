#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

/* Menu screen with widget slots (partial). Offsets are fixed by the original code. */

#ifdef __cplusplus
extern "C" {
#endif

struct DisplayObject {
    float position[3];
    float rotation[3];
    float scale[3];
    unsigned char reserved24[0x6e];
    unsigned char types[2];                 /* 0x92 */
};

struct DisplaySurface;
struct LayoutContainer;
struct LayoutPane;
struct DisplayPart {
    unsigned char reserved00[0x10];
    unsigned char alpha, red, green, blue;
    unsigned char reserved14[8];
    struct LayoutContainer *container;
    unsigned char reserved20[8];
    struct DisplaySurface *surface;
    unsigned char reserved2c[8];
    struct LayoutPane *pane;
};

struct DisplayData {
    struct DisplayObject base;
#ifndef __cplusplus
    struct DisplayPart *parts[];
#endif
};

struct AlignmentHandle;
struct PackedTextState;
struct WidgetBinding {
    unsigned char reserved00[0x28];
    struct AlignmentHandle *alignment;
    unsigned char reserved2c[8];
    struct PackedTextState *text;
};

struct TrainingWidget {
    struct DisplayObject base;
#ifndef __cplusplus
    struct WidgetBinding *bindings[];
#endif
};


struct DisplayPane {
    const void *vtable;
    unsigned char reserved04[0x24];
    float position[3];
    float rotation[3];
    float scale[2];
    float size[2];
    unsigned char reserved50[0x64];
    unsigned char alpha;                    /* 0xb4 */
    unsigned char reservedb5[2];
    unsigned char flags;
};

struct DisplayPaneLink {
    unsigned char reserved00[8];
    struct DisplayPane *pane;
};

struct DisplayBindingTarget {
    unsigned char reserved00[0x10];
    float value;
};
struct DisplayBinding {
    unsigned char reserved00[8];
    float value;
    unsigned char reserved0c[0x14];
    struct DisplayBindingTarget *target;
    unsigned reserved24;
    struct DisplayPaneLink *link;
    unsigned char reserved2c[8];
    struct DisplayPane *pane;
};

struct BoundDisplayObject {
    struct DisplayObject base;
    struct DisplayBinding *bindings[5];
    int screen;
    int index;
    struct BoundDisplayObject *next;
    struct BoundDisplayObject *next_secondary;
    unsigned char reservedb8[8];
    unsigned char queued;
};

struct DisplayObjectList {
    unsigned char reserved00[0x2c];
    struct BoundDisplayObject *head;
};

/* A resource slot contains a leading resource pointer and the queue object
 * prefix at +4. The minimum queue prefix is 0xc4 bytes, not a guessed 0xd0.
 * The final 12 slot bytes stay reserved until their users are integrated. */
struct ResourceRegistrationSlot {
    void *resource;
    struct BoundDisplayObject display;
    unsigned char reservedc8[0x0c];
};
struct ResourceRegistrationGroup {
    unsigned reserved;
    struct ResourceRegistrationSlot entries[8];
};
struct ResourceRegistrationState {
    unsigned char reserved00[0x18];
    struct ResourceRegistrationGroup groups[2];
};
struct MenuOptionGroup;
struct SelectionList;
struct AlternateSelectionList;
struct ScreenInitializationState;
struct W8DisplaySet;
struct WidgetActionState {
    unsigned char reserved00[0xbc];
    unsigned active;
};
struct DisplayQueueFlags {
    unsigned char reserved00[2];
    unsigned char enabled;
};
struct DisplayQueueCounter {
    unsigned char reserved00[8];
    int count;
};
struct DisplayTexCoords {
    float uv[4][2];
};

struct DisplayVertexArray {
    unsigned char capacity;
    unsigned char count;
    unsigned short reserved02;
    struct DisplayTexCoords *entries;
};

struct DisplayVertexPane {
    struct DisplayPane pane;
    unsigned char reservedb8[0x98];
    struct DisplayVertexArray vertices;
};

struct DisplayFlagSlot {
    unsigned int reserved00;
    unsigned int flags;
};

struct FlagDisplayObject {
    struct DisplayObject base;
#ifndef __cplusplus
    struct DisplayFlagSlot *slots[];
#endif
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

struct W8DisplayHandle {
    struct DisplayObject *object;
    unsigned char reserved04[8];
    float scale_x;
    float scale_y;
};

struct W8DisplaySet {
    unsigned char reserved00[0x1300];
#ifndef __cplusplus
    struct W8DisplayHandle *handles[];
#endif
};

struct AnimationContext {
    unsigned int reserved00;
    const struct NameTable *table;
};

struct WidgetContainer {
    unsigned char reserved00[8];
    struct DisplayHandle *handle;
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
extern void func_00176b24(struct SelectionList *selection);
extern void func_0017697c(struct AlternateSelectionList *selection);
extern void func_00175b8c(void *screen);
extern void func_00179160(struct W8DisplaySet *set, int index, float x, float y);
extern void func_00175ab8(void *screen, int index,
                         int red, int green, int blue, int alpha);
extern void func_00166eac(void *screen, int index, float x, float y);
extern void func_00175848(void);
extern void func_001e807c(struct ScreenInitializationState *state);
extern void func_001adabc(struct WidgetActionState *state);
extern void func_00196230(int screen, int index);
extern void func_00196144(int screen, int index);
extern void func_00176d98(void);
extern void func_00177084(void);
extern void func_00176df4(void);
extern void func_00181448(struct BoundDisplayObject *object);
extern void func_0019637c(struct BoundDisplayObject *object,
                          struct BoundDisplayObject **head, int primary);
extern const struct NameTable *bss_00246f4c[];
extern void func_0016e7e0(struct WidgetController *controller, int index, int enabled);
extern void func_0016e3e0(struct WidgetController *controller, int index, int value);
extern void func_0016e648(struct WidgetController *controller, int index, int character);
extern struct AnimationContext bss_00246b48;
extern void func_0014d02c(struct DisplayVertexPane *pane, unsigned index,
                         const struct DisplayTexCoords *coordinates);
extern void func_0014e504(struct DisplayVertexPane *pane, unsigned count);
extern void func_0014e50c(struct DisplayVertexArray *vertices, unsigned count);
extern void func_0015e948(struct DisplayObject *object, int active);
extern void func_0015e954(struct DisplayPaneLink *link, int active);
extern void func_00180358(struct DisplayObject *object, int first, int second);
extern void func_0015e684(struct BoundDisplayObject *object, int index, float x, float y, float z);
extern void func_0018271c(void *object, unsigned char alpha);
extern void func_00181700(struct BoundDisplayObject *object, float x, float y, float z);
extern void func_00181734(struct BoundDisplayObject *object, float width, float height);
extern void func_001826fc(struct DisplayObject *object, int active);
extern void func_00195a08(struct DisplayObject *object, int index, int active);
extern void func_001ad6ec(struct WidgetContainer *container, struct WidgetPosition position);
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
extern void func_001816e0(struct BoundDisplayObject *object, float x, float y);
extern void func_0015e474(struct DisplayObject *object, int index, float x, float y, float z);
extern void func_0015e684(struct BoundDisplayObject *object, int index, float x, float y, float z);
extern void func_001963f8(struct BoundDisplayObject *object);
extern struct BoundDisplayObject *func_00145b5c(int screen, int index);
extern struct BoundDisplayObject *data_00242248;
extern struct BoundDisplayObject *data_0024224c;
extern struct BoundDisplayObject *data_00242250;
extern void func_0018695c(struct BoundDisplayObject *object,
                          struct BoundDisplayObject **head, int primary);
extern void func_001eafa0(void *screen, int index, const unsigned short *text);
extern void func_0016de90(struct Screen *screen, int index);

#ifdef __cplusplus
}
#endif

#endif
