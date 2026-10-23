#include <stddef.h>
#include <string.h>
#include "game/screen.h"
#include "game/selection.h"
#include "game/animation.h"
#include "game/notification.h"
#include "game/deferred_callback.h"
#include "game/item_pages.h"
#include "game/radar.h"
#include "game/pane_descriptors.h"
#include "game/layout.h"

/* [INFERENCE] This neighbourhood shares display and parameter-building
 * operations. Plain static helpers preserve ARMCC's late-inlining context;
 * they are not separately placed code. */
struct LabelStrip { DisplayHandle *handles[9]; };
struct DisplayColor {
    unsigned char components[4];
    DisplayColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    { components[0]=r; components[1]=g; components[2]=b; components[3]=a; }
};
struct LayoutPaneVTable {
    const void *reserved[4];
    void (*set_vertex_color)(LayoutPane *, unsigned, const DisplayColor &);
};
/* [INFERENCE] Typed view of the canonical pane's opaque leading storage. */
struct ColorPaneView { const LayoutPaneVTable *vtable; };
struct LayoutChildList { unsigned count; LayoutListNode root; };
struct LayoutTreeHeader {
    const void *vtable;
    LayoutListNode link;
    LayoutTreeHeader *parent;
    LayoutChildList children;
    unsigned char reserved1c[0x0c];
    WidgetPosition position;
    unsigned char reserved30[0x18];
    WidgetPosition size;
};
struct LayoutContainer {
    unsigned char reserved00[0x10];
    LayoutTreeHeader *root;
};
struct SoundChannelW19G { int resource; void *handle; };
struct AudioGlobalsW19G {
    int enabled;
    unsigned char reserved04[0x2f67c];
    SoundChannelW19G channels[4];
};
struct ResetWidgetSlot {
    DisplayObject *object;
    const NameTable *table;
    unsigned short label;
    unsigned short reserved0a;
    unsigned state;
};
struct ResetWidgetPanel {
    int columns;
    int rows;
    unsigned pending[1];
    ResetWidgetSlot *slots[4];
};
struct TextLayout { unsigned char reserved00[0x44]; float width; };
struct TextReceiver;
struct TextReceiverVTable {
    unsigned char reserved00[0x70];
    void (*set_length)(TextReceiver *, unsigned short, int);
    void (*reset)(TextReceiver *);
    void (*set_text)(TextReceiver *, const unsigned short *, int);
};
struct TextReceiver { const TextReceiverVTable *vtable; };
struct TextAlignment { unsigned char reserved00[8]; TextReceiver *receiver; };
struct TextWidgetBinding { unsigned char reserved00[0x28]; TextAlignment *alignment; };
struct TextWidget { unsigned char reserved00[0x94]; TextWidgetBinding *binding; };
struct MenuTransitionState {
    unsigned char reserved00[0x34]; int next_state;
    unsigned char reserved38[0x58]; int selection;
    int values[2]; int previous[2];
};
struct CounterPanel;
struct TransitionController {
    unsigned char reserved00[0x34];
    unsigned char active, paused;
    unsigned char reserved36[2]; float elapsed;
    unsigned char reserved3c[4]; int mode;
};
struct SelectionMenu {
    unsigned char reserved00[0x48];
    CounterPanel *counters; W6ListController *choices;
    W8DisplaySet *display; Screen *buttons; W6Selector *selector;
    W6HandlePanel10 *resources; Screen *navigation;
    unsigned char reserved64[4]; Screen *icons;
    unsigned char reserved6c[8]; WidgetController *effects;
    unsigned char reserved78[0x14]; TransitionController *transition;
    unsigned char reserved90[4]; int phase;
    unsigned char reserved98[8]; int previous_phase;
    unsigned char reserveda4[0x0c]; int selection; int mode;
    int available[12][14]; int counts[12];
};
struct W7hMetadataSelection { unsigned reserved00; int index; };
union W7hCommandArgument { float scalar; unsigned word; };
struct SelectionModes { unsigned char reserved00[0x90]; int active; int modes[7]; };

struct ParameterCollection {
    int bits_per_word;
    int word_count;
};
struct ResourceMaskLayout {
    int bits_per_word; int word_count; unsigned reserved08[11];
    const NameTable *const *labels;
};
struct ParameterLabels47 {
    int bits_per_word; int word_count; unsigned reserved08[12];
    const NameTable *const *labels;
};
struct ParameterLabels138 {
    int bits_per_word; int word_count; unsigned reserved08[6];
    const NameTable *const *labels;
};
struct SceneParameter03 { int identifier; };
struct SceneParameter13 { int identifier; };
struct SceneParameter30 { int identifier; };
struct SceneParameter47 { int identifier; };
struct SceneParameter138 { int identifier; };
struct ResourceChoiceMap { int reserved00; int values[6]; };

/* INTEGRATOR: move to include/game/scene_neighbourhood.h */
extern "C" {
extern const NameTable *bss_00290e2c[];
extern NameTable *bss_00244378[];
extern AudioGlobalsW19G bss_0029f540;
extern TextLayout bss_0049daa4, bss_0049db14;
extern const MemberCallback data_00218648, data_00218630;
extern const ResourceChoiceMap *bss_00246748[];
extern const int *bss_002468c4[];
extern const NameTable *bss_00246790[], *bss_0024680c[];
extern const NameTable *bss_002471f8[];
extern const NameTable *bss_00246968[], *bss_00246a28[];
extern const NameTable *bss_00247254[], *bss_00247480[];
void func_0016bf80(DisplayObject *, int, float);
/* INTEGRATOR: differs from scratch/authors/r317/src/func_001704c8.c:
 * this callee masks full-width RGBA words; the last argument is not a float. */
void func_00167c34(LayoutPane *, unsigned, unsigned, unsigned, unsigned);
LayoutTreeHeader *func_00167ee4(LayoutTreeHeader *, const char *);
void func_00167e08(BoundDisplayObject *, int, unsigned, unsigned, unsigned, unsigned);
DisplayObject *func_0017a818(const NameTable *, int, int, float, float, float);
DisplayData *func_0015e0c8(DisplayData *, int, int, const unsigned short *, unsigned short);
void func_0015e624(DisplayData *, int, float, float);
void func_0015e6b4(DisplayData *, int, int, float);
int func_0015ecf4(TextLayout *, unsigned short *, const unsigned short *, float, float, float, float);
int func_001af7ec(TextLayout *, unsigned short *, const unsigned short *, float, float, float, float);
void func_00167cfc(DisplayObject *, float, float, float, float, float, float, float);
void func_0015e49c(DisplayData *, float, float);
void func_00170014(DisplayData *, unsigned char, unsigned char, unsigned char, unsigned char);
void func_0016fff8(DisplayData *, unsigned char, unsigned char, unsigned char, unsigned char);
void func_001815a8(BoundDisplayObject *, int, int);
void func_0015e780(TrainingWidget *, unsigned);
void func_0015e824(TrainingWidget *, unsigned);
void func_001803d0(DisplayObject *, unsigned char);
void func_00180388(DisplayObject *, unsigned char);
BoundDisplayObject *func_00181748(BoundDisplayObject *, int);
void func_00180404(BoundDisplayObject *, int);
void func_00181100(BoundDisplayObject *, int, int);
int func_0017e970(int);
int func_0017e948(int);
void func_0014d000(BoundDisplayObject *, const DisplayTexCoords *);
void func_0015e5f0(TrainingWidget *, unsigned, float *, float *);
void func_0017433c(int, int);
void func_00181b68(unsigned, int, int);
void func_00173fb4(CounterPanel *);
void func_00173f9c(TransitionController *, int);
void func_00173ef4(Screen *);
void func_001b472c(void *, int);
int func_00173608(void);
void func_00173408(WidgetController *, int);
void func_00173350(int);
int func_00174470(void);
void func_0017a034(CounterPanel *, int, int);
int func_001e5ce0(const ItemPageSet *);
int func_00172cf0(const ItemPageSet *);
int func_001dd3a4(int, int);
void func_001a79fc(W6SelectionScreen *, int);
void func_001a7494(W6SelectionScreen *, int);
void func_001a78a4(SelectionMenu *, int);
void func_0017365c(W6ListController *, int, int, int, int, int);
void func_00173630(W6ListController *, int, int);
void func_00179d48(AnimationOwner *, int, int, int);
int func_00168670(int, int);
void func_00181a4c(unsigned, const void *, const void *, unsigned, W7hCommandArgument);
int func_00171c44(const W6ListController *, int);
int func_00170e38(void);
void *func_0019ffa8(unsigned);
void func_001852f8(void *);
void func_0017a7e4(const NameTable *const *, int, int, unsigned);

void func_0016fcdc(LabelStrip *, int, float, float);
void func_00170030(BoundDisplayObject *, unsigned, unsigned, unsigned, unsigned);
/* [INFERENCE] Retain the observed callable boundary in this neighbourhood pilot. */
__attribute__((noinline)) void func_00170088(DisplayData *, unsigned char, unsigned char, unsigned char, unsigned char);
int func_00170234(int);
void func_00170298(ResetWidgetPanel *);
/* [INFERENCE] Retain the observed callable boundary in this neighbourhood pilot. */
__attribute__((noinline)) void func_001702fc(ResetWidgetPanel *, int, int);
void func_001704c8(DisplayData *, const char *, unsigned, unsigned, unsigned, unsigned);
void func_0017053c(const DisplayData *, const char *, float *, float *);
DisplayData *func_0017060c(const unsigned short *, unsigned short *, unsigned char, unsigned char,
                         unsigned char, unsigned char, int, int, int, int,
                         float, float, float, float, float, float, float);
BoundDisplayObject *func_00170a20(int, int, int, int, float, float, float);
int func_00170c20(TrainingWidget *, int, const unsigned short *, unsigned short *, int);
void func_00170dc0(DisplayHandle **, int, int);
void func_00170e58(Screen *, int, int);
void func_00170f20(int);
void func_00171000(DisplayHandle **, int, float, float);
void func_00171208(MenuTransitionState *, int);
void func_00171268(SelectionMenu *, int);
void func_00171994(MenuTransitionState *, int);
void func_00171a34(W7hMetadataSelection *, int, int, unsigned);
void func_00171a98(W6SelectionScreen *, int);
int func_00171bf0(int, int);
void func_00171d40(AnimationOwner *, int, int, int);
int func_00171d88(const AnimationOwner *, int, int);
void func_00171e44(ParameterCollection *);
void func_00172034(ParameterCollection *, const SceneParameter03 *const *);
void func_001721e4(ParameterCollection *, const SceneParameter30 *const *);
void func_0017245c(ParameterCollection *, const SceneParameter13 *const *);
void func_001725f4(ParameterCollection *, const SceneParameter47 *const *);
void func_00172818(ParameterCollection *, const SceneParameter138 *const *);
SceneParameter30 *func_001721d8(SceneParameter30 *);
SceneParameter13 *func_00172450(SceneParameter13 *);
SceneParameter47 *func_001725e8(SceneParameter47 *);
SceneParameter138 *func_0017280c(SceneParameter138 *);
void func_001701b4(WidgetController *, int, int);
void func_001705a8(const RadarWidget *, float, float, float, float);
/* These real reset consumers provide shared-helper context, not new credit. */
void func_001aeb30(ResetWidgetPanel *);
void func_001aef6c(ResetWidgetPanel *, unsigned);
void func_00183208(DisplayObject *);
void func_0017a938(const NameTable *const *, int, int);
}

static __forceinline DisplayPart *const *display_parts(const DisplayData *object)
{ return reinterpret_cast<DisplayPart *const *>(object + 1); }
static __forceinline Animation *const *owner_animations(const AnimationOwner *owner)
{ return reinterpret_cast<Animation *const *>(owner + 1); }
static __forceinline TransformEntry (*saved_transforms(SavedNotification *saved))[4]
{ return reinterpret_cast<TransformEntry (*)[4]>(saved + 1); }
static __forceinline unsigned *collection_words(ParameterCollection *collection)
{ return reinterpret_cast<unsigned *>(collection + 1); }

static void set_active(DisplayObject *object, int active)
{
    if (object) {
        switch (func_0017a9a0(object, 0)) {
        case 3: func_00195a08(object, 0, active); break;
        case 6: func_001826fc(object, active); break;
        case 5: func_0015e948(object, active); break;
        }
    }
}
static void set_label(DisplayHandle *handle, int index, int active)
{
    if (handle && handle->object) {
        set_active(handle->object, active);
        if (handle->object && bss_00290e2c[index]->count > 0) {
            func_0017a968(handle->object, bss_00290e2c[index], 0, 0);
            func_0016bf80(handle->object, 0, 100.0f);
        }
    }
}
static void set_position(DisplayObject *object, float x, float y)
{
    /* ORIGINAL BUG: a nonnull handle is assumed to own an object. */
    object->position[0] = x;
    object->position[1] = y;
    switch (func_0017a9a0(object, 0)) {
    case 6: func_00181700((BoundDisplayObject *)object,
        object->position[0], object->position[1], object->position[2]); break;
    case 5: func_0015e684((BoundDisplayObject *)object, 0,
        object->position[0], object->position[1], object->position[2]); break;
    }
}
static __forceinline LayoutTreeHeader *from_link(LayoutListNode *link)
{
    return reinterpret_cast<LayoutTreeHeader *>(reinterpret_cast<char *>(link) -
        offsetof(LayoutTreeHeader, link));
}
static __forceinline LayoutTreeHeader *first_child(LayoutTreeHeader *pane)
{ return pane->children.count != 0 ? from_link(pane->children.root.next) : 0; }
static __forceinline LayoutTreeHeader *next_child(LayoutTreeHeader *pane, LayoutTreeHeader *child)
{
    LayoutListNode *link = child ? child->link.next : pane->children.root.next;
    return link == &pane->children.root ? 0 : from_link(link);
}
static void set_pane_color(LayoutPane *pane, unsigned char red,
                          unsigned char green, unsigned char blue, unsigned char alpha)
{
    DisplayColor color(red, green, blue, alpha);
    ColorPaneView *view=reinterpret_cast<ColorPaneView *>(pane);
    view->vtable->set_vertex_color(pane, 0, DisplayColor(color));
    view->vtable->set_vertex_color(pane, 1, DisplayColor(color));
    view->vtable->set_vertex_color(pane, 2, DisplayColor(color));
    view->vtable->set_vertex_color(pane, 3, DisplayColor(color));
}
static void reset_slot(ResetWidgetSlot *slot)
{
    if (slot) {
        slot->object=0;
        slot->table=0;
        slot->label=255;
        slot->state=0;
    }
}
static void reset_name(DisplayHandle *handle, int index)
{
    DisplayObject *object=handle->object;
    if (object && bss_00290e98[index]->count>0)
        func_0017a968(object,bss_00290e98[index],0,0);
}
static void set_handle_active(DisplayHandle *handle, int active)
{ set_active(handle->object,active); }
static int copy_text(unsigned short *output, const unsigned short *input)
{
    int i = 0;
    unsigned short c = input[0];
    output[0] = c;
    while (c != 0) {
        ++i;
        c = input[i];
        output[i] = c;
    }
    return i;
}

static unsigned short text_length(const unsigned short *text)
{
    unsigned length = 0;
    while (text[length] != 0)
        ++length;
    return (unsigned short)length;
}

static void set_color(struct DisplayObject *object,
                                    unsigned char red, unsigned char green,
                                    unsigned char blue, unsigned char alpha)
{
    int i;
    switch (func_0017a9a0(object, 0)) {
    case 3:
        func_00170088((DisplayData *)object, red, green, blue, alpha);
        break;
    case 6:
        for (i = 0; i < 4; ++i)
            func_00167e08((struct BoundDisplayObject *)object, i, red, green, blue, alpha);
        break;
    case 5:
        func_00170014((struct DisplayData *)object, red, green, blue, alpha);
        func_0016fff8((struct DisplayData *)object, red, green, blue, alpha);
        break;
    }
}

static void set_alignment(struct DisplayObject *object, int horizontal, int vertical)
{
    if (object) {
        switch (func_0017a9a0(object, 0)) {
        case 5:
            func_0015e780((struct TrainingWidget *)object, (unsigned char)(horizontal + 1));
            func_0015e824((struct TrainingWidget *)object, (unsigned char)(vertical + 1));
            break;
        case 6:
            func_001803d0(object, (unsigned char)(horizontal + 1));
            func_00180388(object, (unsigned char)(vertical + 1));
            break;
        }
    }
}


static void enable_labeled_slot(struct WidgetSlot *slot, int enabled, int index)
{
    struct DisplayObject *object = slot->object;
    const struct NameTable *table;
    if (object == 0)
        return;
    switch (func_0017a9a0(object, 0)) {
    case 3: func_00195a08(object, 0, enabled); break;
    case 6: func_001826fc(object, enabled); break;
    case 5: func_0015e948(object, enabled); break;
    }
    object = slot->object;
    if (object) {
        table = bss_00244378[index];
        if (table->count > 0)
            func_0017a968(object, table, 0, 0);
    }
}


/* [INFERENCE] Pairwise clearing expresses the shared two-word loop body;
 * the original source spelling of its peeled first element is unrecovered. */
static void clear_collection_pairs(ParameterCollection *collection)
{
    if (collection->word_count > 0) {
        int i=0;
        if (collection->word_count & 1)
            collection_words(collection)[i++]=0;
        for (; i<collection->word_count; i+=2) {
            collection_words(collection)[i]=0;
            collection_words(collection)[i+1]=0;
        }
    }
}

static __forceinline void set_collection_bit(ParameterCollection *collection, int value)
{
    collection_words(collection)[value / collection->bits_per_word] |=
        1u << (value % collection->bits_per_word);
}

void func_0016fcdc(struct LabelStrip *strip, int index, float x, float y)
{
    if (index < 0) {
        int active = 0;
        for (int i = active; i < 9; ++i)
            set_label(strip->handles[i], i, active);
    } else if (index < 9) {
        struct DisplayHandle *handle;
        set_label(strip->handles[index], index, 1);
        handle = strip->handles[index];
        if (handle)
            set_position(handle->object, x, y);
    }
}

void func_00170030(struct BoundDisplayObject *object, unsigned int red,
                   unsigned int green, unsigned int blue, unsigned int alpha)
{
    int corner = 0;
    do {
        func_00167e08(object, corner, red, green, blue, alpha);
        ++corner;
    } while (corner < 4);
}

void func_00170088(DisplayData *object, unsigned char red,
                             unsigned char green, unsigned char blue, unsigned char alpha)
{
    LayoutTreeHeader *root = display_parts(object)[0]->container->root;
    func_00167c34(reinterpret_cast<LayoutPane *>(root), red, green, blue, alpha);
    LayoutTreeHeader *child = first_child(root);
    if (child == 0)
        return;
    func_00167c34(reinterpret_cast<LayoutPane *>(child), red, green, blue, alpha);
    while ((child = next_child(root, child)) != 0)
        set_pane_color(reinterpret_cast<LayoutPane *>(child), red, green, blue, alpha);
}

int func_00170234(int channel)
{
    struct AudioGlobalsW19G *state = &bss_0029f540;
    struct SoundChannelW19G *entry;
    int end;
    if (state->enabled != 0) {
        end = 4;
        if (channel != -1)
            end = channel + 1;
        else
            channel = 0;
        entry = &state->channels[channel];
        for (; channel < end; ++channel, ++entry) {
            if (entry->resource != -1)
                return 1;
        }
    }
    return 0;
}

void func_00170298(struct ResetWidgetPanel *panel)
{
    for (int i=0; i<4; ++i) {
        ResetWidgetSlot *slot=panel->slots[i];
        if (slot)
            reset_slot(slot);
    }
    func_001702fc(panel, 1, 5);
    func_001702fc(panel, 3, 10);
}

void func_001702fc(struct ResetWidgetPanel *slots, int index, int resource)
{
    struct ResetWidgetSlot *handle = slots->slots[index];
    if (handle != 0 && handle->object == 0) {
        const struct NameTable *table = bss_0024716c[resource];
        float z;
        int screen = 3;
        handle->table = table;
        switch (index) {
        case 0: z = 0.5f; break;
        case 1: z = 0.7f; break;
        case 2: z = 1.0f; break;
        default: z = 1.3f; screen = 4; break;
        }
        handle->object = func_0017a818(table, screen, 0, 0.0f, 0.0f, z);
    }
}

void func_001704c8(DisplayData *object, const char *name, unsigned red,
                   unsigned green, unsigned blue, unsigned alpha)
{
    LayoutTreeHeader *root = display_parts(object)[0]->container->root;
    LayoutTreeHeader *pane = first_child(root);
    if (name) pane = func_00167ee4(root, name);
    func_00167c34(reinterpret_cast<LayoutPane *>(pane), red, green, blue, alpha);
}

void func_0017053c(const struct DisplayData *object, const char *name, float *width, float *height)
{
    struct LayoutTreeHeader *root = display_parts(object)[0]->container->root;
    struct LayoutTreeHeader *pane = first_child(root);
    if (name != 0)
        pane = func_00167ee4(root, name);
    if (pane != 0) {
        struct WidgetPosition size = pane->size;
        memcpy(width, &size.x, sizeof *width);
        memcpy(height, &size.y, sizeof *height);
        return;
    }
    *width = 0.0f;
    *height = 0.0f;
}

struct DisplayData *func_0017060c(const unsigned short *input, unsigned short *output,
                                 unsigned char red, unsigned char green,
                                 unsigned char blue, unsigned char alpha,
                                 int font, int layer, int screen, int active,
                                 float width, float height, float line_height,
                                 float limit, float x, float y, float z)
{
    struct DisplayData *object = 0;
    int horizontal = -1, vertical = -1;
    if (limit <= 0.0f)
        limit = 240.0f;
    /* The incoming line_height is not used; the layout height is derived from the width. */
    line_height = width * 1.5f;
    if (font) {
        bss_0049daa4.width = limit;
        copy_text(output, input);
        func_0015ecf4(&bss_0049daa4, output, input, width, height, line_height, limit);
        object = func_0015e0c8(0, 0, layer, output, text_length(output));
        func_0015e624(object, 0, width, height);
        func_0015e6b4(object, 0, font, limit);
    } else {
        bss_0049db14.width = limit;
        copy_text(output, input);
        func_001af7ec(&bss_0049db14, output, input, width, height, line_height, 240.0f);
        object = func_0015e0c8(0, 0, layer, input, text_length(output));
        func_0015e624(object, 0, width, height);
        func_0015e6b4(object, 0, 0, 240.0f);
    }
    if (object) {
        func_00167cfc(&object->base, x, y, z, 1.0f, 1.0f, 0.0f, 0.0f);
        func_0015e49c(object, 0.0f, 0.0f);
        set_color(&object->base, red, green, blue, alpha);
        func_001815a8((struct BoundDisplayObject *)object, screen, layer);
        set_alignment(&object->base, horizontal, vertical);
        set_active(&object->base, active);
    }
    return object;
}

struct BoundDisplayObject *func_00170a20(int texture, int screen, int active, int index,
                                         float x, float y, float z)
{
    struct BoundDisplayObject *object = func_00181748(0, 0);
    if (object != 0) {
        struct DisplayTexCoords coordinates;
        int vertex;
        int width, height;
        coordinates.uv[0][0] = 0.0f;
        coordinates.uv[0][1] = 0.0f;
        coordinates.uv[1][0] = 1.0f;
        coordinates.uv[1][1] = 0.0f;
        coordinates.uv[2][0] = 0.0f;
        coordinates.uv[2][1] = 1.0f;
        coordinates.uv[3][0] = 1.0f;
        coordinates.uv[3][1] = 1.0f;
        func_00180404(object, 0);
        func_00181100(object, texture, 0);
        width = func_0017e970(texture);
        height = func_0017e948(texture);
        func_00181734(object, (float)width, (float)height);
        func_0014d000(object, &coordinates);
        func_001803d0(&object->base, 1);
        func_00180388(&object->base, 1);
        for (vertex = 0; vertex < 4; ++vertex)
            func_00167e08(object, vertex, 255, 255, 255, 255);
        func_0018271c(object, 255);
        func_001815a8(object, screen, index);
        func_00167cfc(&object->base, x, y, z, 1.0f, 1.0f, 0.0f, 0.0f);
        if (object != 0) {
            switch (func_0017a9a0(&object->base, 0)) {
            case 3: func_00195a08(&object->base, 0, active); break;
            case 5: func_0015e948(&object->base, active); break;
            case 6: func_001826fc(&object->base, active); break;
            }
        }
    }
    return object;
}

int func_00170c20(TrainingWidget *widget, int mode,
                             const unsigned short *text, unsigned short *output,
                             int reset)
{
    float width = 0.0f;
    float height = 0.0f;
    int length = 0;
    int result;
    TextLayout *workspace = &bss_0049daa4;
    TextReceiver *receiver = ((TextWidget *)widget)->binding->alignment->receiver;
    if (mode) {
        func_0015e5f0(widget, 0, &width, &height);
        result = func_0015ecf4(reinterpret_cast<TextLayout *>(bss_0049daa4.reserved00), output, text,
                              width, height, width * 1.5f, workspace->width);
    } else {
        func_0015e5f0(widget, 0, &width, &height);
        result = func_001af7ec(&bss_0049db14, output, text,
                              width, height, width * 1.5f, 240.0f);
    }
    while (output[length] != 0)
        ++length;
    if (reset) {
        receiver->vtable->reset(receiver);
        receiver->vtable->set_length(receiver, (unsigned short)length, 0);
    }
    receiver->vtable->set_text(receiver, output, 0);
    return result;
}

void func_00170dc0(DisplayHandle **handles, int index, int enabled)
{
    DisplayHandle *handle=handles[index];
    if (handle && handle->object) set_active(handle->object,enabled);
}

void func_00170e58(struct Screen *screen, int index, int enabled)
{
    struct WidgetSlot *slot = screen->slots[index];
    if (slot)
        enable_labeled_slot(slot, enabled, index);
}

void func_00170f20(int group)
{
    int index;
    for (index = 0; index < 4; ++index) {
        struct TransformEntry *entry = &saved_transforms(&bss_00280af0)[group][index];
        entry->reserved00 = 0;
        entry->x = 0.0f;
        entry->y = 0.0f;
        entry->count = 0;
        entry->flags = 0;
    }
}

void func_00171000(struct DisplayHandle **handles, int index, float x, float y)
{
    if (index < 0) {
        int i;
        for (i = 0; i < 10; ++i) {
            struct DisplayHandle *handle = handles[i];
            if (handle != 0) {
                set_handle_active(handle, 0);
                reset_name(handle, i);
            }
        }
    } else if (index < 10) {
        struct DisplayHandle *handle = handles[index];
        if (handle != 0) {
            set_handle_active(handle, 1);
            reset_name(handle, index);
        }
        if (handles[index] != 0) {
            /* ORIGINAL BUG: a nonnull handle with a null object is dereferenced here. */
            set_position(handles[index]->object, x, y);
        }
    }
}


void func_00171208(MenuTransitionState *state, int next_state)
{
    state->next_state=next_state;
    state->values[state->selection]=0;
    func_0017433c(0, (int)func_0017adb4() * 60);
    func_00181b68(7, 1, 1);
    func_00177b10(reinterpret_cast<CallbackTarget *>(state), data_00218648);
}

void func_00171268(struct SelectionMenu *menu, int changed)
{
    if (changed) {
        if (menu->mode != 0 && menu->mode != 3) {
            func_001795cc(menu->navigation, 4, -1);
            func_00170e50(menu->navigation, 5);
            func_00179600(menu->choices, 14, 1);
        } else {
            func_0017a6f4(menu->buttons, 2);
            func_001795cc(menu->navigation, 4, 5);
            func_00179600(menu->choices, 13, 1);
        }
        func_00173fb4(menu->counters);
    }
    func_00178e98(menu->choices, 13);
    func_00178e98(menu->choices, 14);
    switch (menu->previous_phase) {
    case 3:
        func_00179600(menu->choices, 0, 0);
        func_00179600(menu->choices, 15, 0);
        func_00179600(menu->choices, 16, 0);
        func_00173fb4(menu->counters);
        func_00173f9c(menu->transition, 1);
        break;
    case 4:
        func_00179600(menu->choices, 1, 0);
        func_00179600(menu->choices, 2, 0);
        func_00179600(menu->choices, 3, 0);
        func_00179600(menu->choices, 4, 0);
        func_00179600(menu->choices, 5, 0);
        func_00179600(menu->choices, 6, 0);
        func_00179600(menu->choices, 7, 0);
        func_00179600(menu->choices, 8, 0);
        func_00179600(menu->choices, 9, 0);
        func_00179600(menu->choices, 10, 0);
        func_00179600(menu->choices, 11, 0);
        func_00179600(menu->choices, 12, 0);
        func_00179600(menu->choices, 15, 0);
        func_00179600(menu->choices, 16, 0);
        func_00173f9c(menu->transition, 1);
        break;
    case 9:
        func_00173ef4(menu->icons);
        func_00173fb4(menu->counters);
        func_00179600(menu->choices, 14, 0);
        func_0017a0e0(menu->display, 0, 0);
        break;
    }
    switch (menu->phase) {
    case 3: {
        int category, item;
        func_001a79fc((struct W6SelectionScreen *)menu, 0);
        func_00173d8c(menu->resources, 9);
        func_00179600(menu->choices, 0, 1);
        func_00178e98(menu->choices, 0);
        func_0017365c(menu->choices, 0, 8, 1, 0, 0);
        func_00173630(menu->choices, 0, 8);
        for (category = 0; category < 12; ++category) {
            for (item = 0; item < menu->counts[category]; ++item) {
                if (menu->available[category][item] != 0)
                    break;
            }
            if (item >= menu->counts[category])
                func_001780c4(menu->choices, 0, category, 0);
        }
        break;
    }
    case 4: {
        int category, item, any = 0;
        func_001a7494((struct W6SelectionScreen *)menu, 0);
        func_00173d8c(menu->resources, 10);
        func_00179600(menu->choices, func_001718ec((struct W6SelectionScreen *)menu), 1);
        func_00179600(menu->choices, 13, 1);
        func_00179d48((struct AnimationOwner *)menu->choices, 13, 0, 1);
        func_00178e98(menu->choices, func_001718ec((struct W6SelectionScreen *)menu));
        func_0017365c(menu->choices, func_001718ec((struct W6SelectionScreen *)menu), 8, 1, 1, 0);
        if (!changed)
            func_00173630(menu->choices, 1, 8);
        category = func_001795bc();
        for (item = 0; item < menu->counts[category]; ++item) {
            if (menu->available[category][item]) {
                func_001780c4(menu->choices, func_001718ec((struct W6SelectionScreen *)menu), item, 1);
                any = 1;
            } else {
                func_001780c4(menu->choices, func_001718ec((struct W6SelectionScreen *)menu), item, 0);
            }
        }
        func_001780c4(menu->choices, func_001718ec((struct W6SelectionScreen *)menu), item, any);
        break;
    }
    case 9: {
        struct ItemPageSet *set;
        int category, item, any;
        func_00174210(menu->selector, 105);
        func_00173d8c(menu->resources, 4);
        func_001b472c(menu->icons, 4);
        func_0017a0e0(menu->display, 0, 1);
        func_00179600(menu->choices, 14, 1);
        if (func_00173608()) {
            func_00173408(menu->effects, 2);
            if (menu->mode == 1)
                func_00173350(3);
        }
        set = data_0023ecf4;
        func_0017a034(menu->counters, 1, func_00174470());
        func_0017a034(menu->counters, 2, func_001795bc());
        func_0017a034(menu->counters, 3, func_00178144());
        func_0017a034(menu->counters, 4, func_001e5ce0(set));
        func_0017a034(menu->counters, 5, func_00172cf0(set));
        category = func_001795bc();
        item = func_001dd3a4(category, func_00178144());
        if (menu->available[category][item]) {
            func_001780c4(menu->choices, 14, 3, 1);
        } else {
            any = 0;
            if (func_00178144() == 31) {
                for (item = 0; item < menu->counts[category]; ++item) {
                    if (menu->available[category][item])
                        any = 1;
                }
            }
            if (any)
                func_001780c4(menu->choices, 14, 3, 1);
            else
                func_001780c4(menu->choices, 14, 3, 0);
        }
        return;
    }
    default:
        return;
    }
    func_001a78a4(menu, 0);
    func_00173f9c(menu->transition, 0);
}


void func_00171994(MenuTransitionState *state, int next_state)
{
    state->next_state=next_state;
    state->values[state->selection]=0;
    func_0017433c(0, (int)func_0017adb4() * 60);
    func_00181b68(7, 1, 1);
    func_00177b10(reinterpret_cast<CallbackTarget *>(state), data_00218630);
}

void func_00171a34(struct W7hMetadataSelection *selection, int identifier,
                   int year, unsigned flags)
{
    int index = func_00168670(identifier, year);
    if (index >= 0) {
        const struct MetadataEntry *entry = &data_0023fce4[index];
        union W7hCommandArgument argument;
        selection->index = index;
        argument.word = 4;
        func_00181a4c((unsigned)entry->header, entry, entry, flags, argument);
    }
}

void func_00171a98(struct W6SelectionScreen *screen, int use_active)
{
    float x = 0.0f;
    float y = 0.0f;
    float maximum;
    int list;
    const struct SelectionModes *const modes = (const struct SelectionModes *)screen;
    const int mode = use_active ? modes->modes[modes->active] : modes->modes[0];
    if (mode == 2) {
        maximum = 1056.0f;
        list = 1;
    } else if (mode == 3) {
        list = func_00171c7c(screen);
        maximum = (func_00171c44(screen->list, list) - 1) * 96.0f;
    } else {
        return;
    }
    func_00178fb0((const AnimationOwner *)screen->list, list, &x, &y);
    if (func_00174324((const AnimationOwner *)screen->list, 16)) {
        if (x <= 0.0f)
            func_00179600(screen->list, 16, 0);
    } else {
        if (x > 0.0f)
            func_00179600(screen->list, 16, 1);
    }
    if (func_00174324((const AnimationOwner *)screen->list, 17)) {
        if (x >= maximum)
            func_00179600(screen->list, 17, 0);
    } else {
        if (x < maximum)
            func_00179600(screen->list, 17, 1);
    }
}

int func_00171bf0(int grade, int category)
{
    int i;
    for (i = 0; i < 20; ++i) {
        if (bss_00280af8.storage.fields.history[grade].lists[category][i] == 1)
            break;
    }
    return i;
}



void func_00171d40(struct AnimationOwner *owner, int index, int child_index, int text)
{
    AnimationChild *child=0;
    if (owner_animations(owner)[index] && child_index>=0)
        child=owner_animations(owner)[index]->children[child_index];
    if (child) {
        struct {
            struct DisplayObject *object;
            const struct NameTable *table;
        } binding;
        memcpy(&binding, child, sizeof binding);
        func_0017a968(binding.object, binding.table, text, 0);
        child->text_index = text;
    }
}

int func_00171d88(const struct AnimationOwner *owner, int index, int child_index)
{
    int text=-1;
    if (owner_animations(owner)[index] && child_index>=0) {
        const AnimationChild *child=owner_animations(owner)[index]->children[child_index];
        if (child)
            text=child->text_index;
    }
    return text;
}

void func_00171e44(ParameterCollection *collection)
{
    clear_collection_pairs(collection);
    for (int i=0; i<5; ++i) set_collection_bit(collection,i);
    const NameTable **selected = (const NameTable **)func_0019ffa8(6 * sizeof(*selected));
    /* ORIGINAL BUG: allocation failure is not checked before these writes. */
    for (int i=0; i<6; ++i) selected[i]=0;
    int selected_count=0;
    int item=0;
    for (int group=0; group<collection->word_count; ++group) {
        for (int bit=0; bit<collection->bits_per_word; ++bit) {
            if (collection_words(collection)[group] & (1u<<bit))
                selected[selected_count++]=bss_002466f8[item];
            ++item;
            if (item>=5) break;
        }
    }
    if (selected_count>0) {
        selected[selected_count]=bss_002466f8[5];
        func_0017a7e4(selected,-1,3,0x80);
    }
    func_001852f8(selected);
}

void func_00172034(ParameterCollection *collection, const SceneParameter03 *const *parameters)
{
    if (!parameters) return;
    clear_collection_pairs(collection);
    for (int i=0; i<3; ++i) {
        int identifier=parameters[i]->identifier;
        if (identifier==-1) break;
        const PaneRange *mapping=data_0023ed08[identifier];
        if (mapping->identifier != -1) {
            set_collection_bit(collection,mapping->identifier);
            if (mapping->kind != -1)
                set_collection_bit(collection,mapping->kind);
        }
    }
    const NameTable **selected = (const NameTable **)func_0019ffa8(4 * sizeof(*selected));
    /* ORIGINAL BUG: allocation failure is not checked before these writes. */
    for (int i=0; i<4; ++i) selected[i]=0;
    int selected_count=0;
    int item=0;
    for (int group=0; group<collection->word_count; ++group) {
        for (int bit=0; bit<collection->bits_per_word; ++bit) {
            if (collection_words(collection)[group] & (1u<<bit))
                selected[selected_count++]=bss_00247140[item];
            ++item;
            if (item>=3) break;
        }
    }
    if (selected_count>0) {
        selected[selected_count]=bss_00247140[3];
        func_0017a7e4(selected,-1,3,0x80);
    }
    func_001852f8(selected);
}

void func_001721e4(ParameterCollection *collection, const SceneParameter30 *const *parameters)
{
    if (!parameters) return;
    clear_collection_pairs(collection);
    if (func_00170e38())
        ((ResourceMaskLayout *)collection)->labels=bss_00246790;
    else
        ((ResourceMaskLayout *)collection)->labels=bss_0024680c;
    for (int i=0; i<30; ++i) {
        int identifier=parameters[i]->identifier;
        if (identifier==-1) break;
        const ResourceChoiceMap *mapping=bss_00246748[identifier];
        for (int j=0; j<6; ++j) {
            int value=mapping->values[j];
            if (value==-1) break;
            set_collection_bit(collection,value);
        }
    }
    const NameTable **selected = (const NameTable **)func_0019ffa8(31 * sizeof(*selected));
    /* ORIGINAL BUG: allocation failure is not checked before these writes. */
    for (int i=0; i<31; ++i) selected[i]=0;
    int selected_count=0;
    int item=0;
    for (int group=0; group<collection->word_count; ++group) {
        for (int bit=0; bit<collection->bits_per_word; ++bit) {
            if (collection_words(collection)[group] & (1u<<bit))
                selected[selected_count++]=((ResourceMaskLayout *)collection)->labels[item];
            ++item;
            if (item>=30) break;
        }
    }
    if (selected_count>0) {
        selected[selected_count]=((ResourceMaskLayout *)collection)->labels[30];
        func_0017a7e4(selected,-1,3,0x80);
    }
    func_001852f8(selected);
}

void func_0017245c(ParameterCollection *collection, const SceneParameter13 *const *parameters)
{
    if (!parameters) return;
    clear_collection_pairs(collection);
    for (int i=0; i<13; ++i) {
        int value=parameters[i]->identifier;
        if (value==-1) break;
        set_collection_bit(collection,value);
    }
    const NameTable **selected = (const NameTable **)func_0019ffa8(14 * sizeof(*selected));
    /* ORIGINAL BUG: allocation failure is not checked before these writes. */
    for (int i=0; i<14; ++i) selected[i]=0;
    int selected_count=0;
    int item=0;
    for (int group=0; group<collection->word_count; ++group) {
        for (int bit=0; bit<collection->bits_per_word; ++bit) {
            if (collection_words(collection)[group] & (1u<<bit))
                selected[selected_count++]=bss_002471f8[item];
            ++item;
            if (item>=13) break;
        }
    }
    if (selected_count>0) {
        selected[selected_count]=bss_002471f8[13];
        func_0017a7e4(selected,-1,3,0x80);
    }
    func_001852f8(selected);
}

void func_001725f4(ParameterCollection *collection, const SceneParameter47 *const *parameters)
{
    if (!parameters) return;
    if (func_00170e38())
        ((ParameterLabels47 *)collection)->labels=bss_00246968;
    else
        ((ParameterLabels47 *)collection)->labels=bss_00246a28;
    for (int i=0; i<collection->word_count; ++i)
        collection_words(collection)[i]=0;
    for (int i=0; i<47; ++i) {
        int identifier=parameters[i]->identifier;
        if (identifier==-1) break;
        const int *mapping=bss_002468c4[identifier];
        for (int j=0; j<5; ++j) {
            int value=mapping[j];
            if (value==-1) break;
            set_collection_bit(collection,value);
        }
    }
    const NameTable **selected = (const NameTable **)func_0019ffa8(48 * sizeof(*selected));
    /* ORIGINAL BUG: allocation failure is not checked before these writes. */
    for (int i=0; i<48; ++i) selected[i]=0;
    int selected_count=0;
    int item=0;
    for (int group=0; group<collection->word_count; ++group) {
        for (int bit=0; bit<collection->bits_per_word; ++bit) {
            if (collection_words(collection)[group] & (1u<<bit))
                selected[selected_count++]=((ParameterLabels47 *)collection)->labels[item];
            ++item;
            if (item>=47) break;
        }
    }
    if (selected_count>0) {
        selected[selected_count]=((ParameterLabels47 *)collection)->labels[47];
        func_0017a7e4(selected,-1,3,0x80);
    }
    func_001852f8(selected);
}

void func_00172818(ParameterCollection *collection, const SceneParameter138 *const *parameters)
{
    if (!parameters) return;
    if (func_00170e38())
        ((ParameterLabels138 *)collection)->labels=bss_00247254;
    else
        ((ParameterLabels138 *)collection)->labels=bss_00247480;
    int word_count=collection->word_count;
    for (int i=0; i<word_count; ++i)
        collection_words(collection)[i]=0;
    for (int i=0; i<138; ++i) {
        int value=parameters[i]->identifier;
        if (value==-1) break;
        set_collection_bit(collection,value);
    }
    const NameTable **selected = (const NameTable **)func_0019ffa8(139 * sizeof(*selected));
    /* ORIGINAL BUG: allocation failure is not checked before these writes. */
    for (int i=0; i<139; ++i) selected[i]=0;
    int selected_count=0;
    int item=0;
    for (int group=0; group<collection->word_count; ++group) {
        for (int bit=0; bit<collection->bits_per_word; ++bit) {
            if (collection_words(collection)[group] & (1u<<bit))
                selected[selected_count++]=((ParameterLabels138 *)collection)->labels[item];
            ++item;
            if (item>=138) break;
        }
    }
    if (selected_count>0) {
        selected[selected_count]=((ParameterLabels138 *)collection)->labels[138];
        func_0017a7e4(selected,-1,3,0x80);
    }
    func_001852f8(selected);
}

SceneParameter30 *func_001721d8(SceneParameter30 *parameter)
{ parameter->identifier=-1; return parameter; }
SceneParameter13 *func_00172450(SceneParameter13 *parameter)
{ parameter->identifier=-1; return parameter; }
SceneParameter47 *func_001725e8(SceneParameter47 *parameter)
{ parameter->identifier=-1; return parameter; }
SceneParameter138 *func_0017280c(SceneParameter138 *parameter)
{ parameter->identifier=-1; return parameter; }

void func_001701b4(WidgetController *controller, int index, int enabled)
{
    WidgetSlot *slot=widget_slots(controller)[index];
    if (slot) set_active(slot->object,enabled);
}
void func_001705a8(const struct RadarWidget *widget, float red, float green, float blue, float alpha)
{
    struct RadarChart *chart;
    int index;
    if (!widget)
        return;
    chart = func_00167cec(widget, 0);
    if (chart)
        chart->alpha.value = alpha;
    /* ORIGINAL BUG: a missing chart is nevertheless dereferenced by this loop. */
    for (index = 0; index < 16; ++index) {
        chart->colors[index].red = red;
        chart->colors[index].green = green;
        chart->colors[index].blue = blue;
        chart->colors[index].alpha = chart->alpha;
    }
}

void func_001aeb30(ResetWidgetPanel *screen)
{
    for (int i=0; i<4; ++i) {
        if (screen->slots[i]) {
            if (screen->slots[i]->object)
                func_00183208(screen->slots[i]->object);
            reset_slot(screen->slots[i]);
        }
    }
    const NameTable **tables=(const NameTable **)func_0019ffa8(12*sizeof(*tables));
    /* ORIGINAL BUG: allocation failure is not checked before these writes. */
    for (int i=0; i<12; ++i)
        tables[i]=0;
    int count=0, index=0;
    for (int row=0; row<screen->rows; ++row) {
        for (int column=0; column<screen->columns; ++column) {
            if (screen->pending[row] & (1u<<column))
                tables[count++]=bss_0024716c[index];
            if (++index>=11)
                break;
        }
    }
    if (count>0) {
        tables[count]=bss_0024716c[11];
        func_0017a938(tables,-1,0);
    }
    for (int row=0; row<screen->rows; ++row)
        screen->pending[row]=0;
    func_001852f8(tables);
}

void func_001aef6c(ResetWidgetPanel *screen, unsigned record)
{
    /* ORIGINAL BUG: the first slot is dereferenced before its null check. */
    DisplayObject *object=screen->slots[0]->object;
    if (object && screen->slots[0]) {
        func_00183208(object);
        reset_slot(screen->slots[0]);
    }
    if (record<5) {
        func_001702fc(screen,0,record);
        if (screen->slots[0]->object) {
            func_0017a968(screen->slots[0]->object,bss_0024716c[record],0,0);
            func_001701b4(reinterpret_cast<WidgetController *>(screen),0,1);
        }
    }
}
