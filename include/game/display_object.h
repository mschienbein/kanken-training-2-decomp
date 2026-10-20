#ifndef GAME_DISPLAY_OBJECT_H
#define GAME_DISPLAY_OBJECT_H

struct ResourceEntry;
struct DisplayObject;

/* Display-model D01-D04: 00135268 allocates 0xcc-byte pool entries,
 * 00183168 returns them, and 00183208/0019d3b4 release the same owners. */
struct DisplayAttachmentCommon {
    unsigned in_use;
    unsigned flags;
    float value;
    float rate;
    unsigned char alpha, red, green, blue;
};

/* Real initial record shared by the timed tag-3/5/6 payloads, not tag 1. */
struct DisplayTimedAttachment {
    struct DisplayAttachmentCommon common;
    int mode;
    float duration;
};

struct DisplayObject {
    float position[3];
    float rotation[3];
    float scale[3];
    unsigned char unknown24[0x20];
    unsigned state44;
    int kind_identifier;
    unsigned char unknown4c[0x40];
    unsigned char ordering_a[3];
    unsigned char ordering_b[3];
    unsigned char types[2];
    struct DisplayAttachmentCommon *attachments[2];
    unsigned state9c;
    unsigned active;
    unsigned statea4;
    int screen;
    int index;
    struct DisplayObject *next;
    struct DisplayObject *next_secondary;
    unsigned stateb8;
    unsigned statebc;
    unsigned char queued;
    unsigned char unknownc1[3];
    int pool_index;
    unsigned generation;
};

struct TextDisplayAttachment {
    struct DisplayTimedAttachment timed;
    unsigned state1c, state20, state24;
    struct ResourceEntry *surface;
};

/* Tags 3/5/6: 0013566c/0015e0c8/00181748 allocate 0x58/0x2c/0x3c
 * payloads with this real initial record (display-model D05-D07). */
#define display_timed_attachment(object, index) \
    ((struct DisplayTimedAttachment *)(object)->attachments[index])

static __inline const struct DisplayTimedAttachment *display_timed_attachment_const(
    const struct DisplayObject *object, int index)
{
    /* Same producer-established timed-prefix precondition as above. */
    return (const struct DisplayTimedAttachment *)object->attachments[index];
}

static __inline struct TextDisplayAttachment *display_text_attachment(
    const struct DisplayObject *object, int index)
{
    /* Tag 5: 0015e0c8 allocates a 0x2c-byte text payload and installs
     * 0015db7c's ResourceEntry at +0x28 (display-model D07). */
    return (struct TextDisplayAttachment *)object->attachments[index];
}

#ifdef __cplusplus
namespace nw { namespace lyt {
class Layout;
class AnimTransform;
class ResourceAccessor;
class Picture;
class Material;
} }

struct LayoutDisplayAttachment {
    DisplayTimedAttachment timed;
    nw::lyt::Layout *layout;
    nw::lyt::AnimTransform *target;
    nw::lyt::AnimTransform **animations;
    int state28, state2c, count;
    nw::lyt::ResourceAccessor *accessor;
    int resource_count;
    void *resource_storage; /* Allocated pointer-vector storage; element contract unresolved. */
    unsigned state40, state44, state48, state4c, state50, state54;
};

struct PaneDisplayAttachment {
    DisplayTimedAttachment timed;
    unsigned state1c, state20, state24;
    int mode;
    int resource;
    unsigned state30;
    nw::lyt::Picture *picture;
    nw::lyt::Material **material;
};

static __inline LayoutDisplayAttachment *display_layout_attachment(
    const DisplayObject *object, int index)
{
    /* Tag 3: 0013566c allocates a 0x58-byte layout payload, constructs
     * Layout at +0x1c and ResourceAccessor at +0x34 (display-model D05). */
    return (LayoutDisplayAttachment *)object->attachments[index];
}

static __inline PaneDisplayAttachment *display_pane_attachment(
    const DisplayObject *object, int index)
{
    /* Tag 6: 00181748 allocates a 0x3c-byte pane payload; 0014b4b0
     * constructs its 0x158-byte Picture at +0x34 (display-model D06). */
    return (PaneDisplayAttachment *)object->attachments[index];
}
#endif

#endif
