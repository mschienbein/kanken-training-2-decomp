#ifndef GAME_SOUND_TRACK_H
#define GAME_SOUND_TRACK_H

struct AudioVoiceGroup;
struct BufferPosition;
struct BufferedMemoryStream;
struct DecoderState;
struct EncodedByteCount;
struct ExtendedState;
struct FileStreamObject;
struct ForwardedStream;
struct ForwardedStreamVTable;
struct ForwardingStream;
struct FreeListNode;
struct InitializedState;
struct ModeTableState;
struct ObjectPool;
struct PoolLink;
struct ResourceFontView;
struct ResourceMember13c;
struct SelectionOwner;
struct SoundTrack;
struct SoundTrackPool;
struct StorageArena;
struct StreamRange;
struct TaskPool;

#include <stdbool.h>
#include "game/pool_list.h"
#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct SoundTrack {
    const void *vtable;
    unsigned char reserved04[0x94];
    unsigned char stopped;
    unsigned char reserved99[0x93];
    void *resource;
    struct SoundTrack *children;
    void *owner;
    unsigned char enabled, active, pending;
    unsigned char reserved13b[0x65];
    void (*callback)(struct SoundTrack *, unsigned event, void *context);
    void *callback_data;
    struct AudioVoiceGroup *task;
    struct SoundTrack *next;
    struct PoolLink link;
};

struct BufferedMemoryStream {
    unsigned char reserved00[0x678];
    unsigned char *buffer;
    unsigned int capacity;
};

struct ForwardedStreamVTable {
    unsigned char reserved00[0x38];
    unsigned int (*query)(struct ForwardedStream *stream);
};

struct ForwardedStream {
    const struct ForwardedStreamVTable *vtable;
};

struct ForwardingStream {
    unsigned char reserved00[0x14];
    struct ForwardedStream *stream;
};

struct InitializedState {
    unsigned char reserved00[0x118];
    unsigned int initialized;
};

struct DecoderState {
    unsigned char reserved00[8];
    unsigned int first_state;
    unsigned int second_state;
};

struct EncodedByteCount {
    unsigned char reserved00[0x168];
    unsigned char count_minus_one;
};

struct ResourceMember13c {
    unsigned char reserved00[0x13c];
    void *resource;
};

struct BufferPosition {
    unsigned char reserved00[0x3c];
    unsigned int limit;
    unsigned int position;
};

struct SelectionOwner {
    unsigned char reserved00[0xa0];
    int selection;
    unsigned char reserveda4[8];
    int identifier;
    unsigned char reservedb0[0x0c];
    unsigned flags;
};

struct ModeTableState {
    unsigned char reserved00[0x2c];
    unsigned char mode;
    unsigned char reserved2d[7];
    unsigned char *table;
};

struct ExtendedState {
    unsigned char reserved00[0x3040];
    unsigned active;
};

struct StreamRange {
    const void *vtable;
    unsigned char enabled;
    unsigned char reserved05[7];
    unsigned field0c;
    unsigned field10;
    struct FileStreamObject *owner;
    void *buffer;
    unsigned size;
    int position;
    int limit;
};

struct FreeListNode {
    struct FreeListNode *next;
};

struct SoundTrackPool {
    unsigned reserved00;
    void *owner;
    struct FreeListNode *free_list;
};

#ifdef __cplusplus
extern "C" {
#endif
extern void func_001dace8(struct BufferedMemoryStream *stream);
extern void func_0016b260(void *allocation);
extern struct StorageArena *func_001db07c(struct StorageArena *arena,
                                        unsigned int capacity);
extern unsigned func_00130308(void);
extern void func_0018ef04(void);
extern const unsigned data_00224964[];
extern struct SoundTrack *func_001d540c(struct SoundTrack *track);
extern struct FreeListNode *func_0017dfdc(struct FreeListNode **head);
extern struct SoundTrack *func_001d607c(struct SoundTrack *track);
extern struct SoundTrack *func_001d5528(struct ObjectPool *pool);
extern struct AudioVoiceGroup *func_0017e500(struct TaskPool *pool, unsigned first, unsigned second,
                                     void (*callback)(struct AudioVoiceGroup *, int, void *),
                                     void *context);
extern void func_001d96e8(struct SoundTrack *track, unsigned first, unsigned second);
extern void func_0016b248(struct BufferedMemoryStream *stream);
extern unsigned int func_001d3f08(struct ForwardingStream *object);
extern bool func_001d9b9c(const struct InitializedState *state);
extern struct StorageArena *func_001da358(struct StorageArena *arena,
                                 unsigned int capacity);
extern bool func_001dcae0(const struct DecoderState *decoder);
extern bool func_001dcebc(const struct DecoderState *decoder);
extern signed char func_001efe80(const struct ResourceFontView *font);
extern unsigned short func_001efe9c(const struct ResourceFontView *font);
extern unsigned char func_001f0320(const struct EncodedByteCount *object);
extern void *func_001f1174(const struct ResourceMember13c *object, unsigned int index);
extern bool func_001f1184(const struct ResourceMember13c *object);
extern unsigned int func_001f18a8(const struct BufferPosition *buffer);
extern void func_0018eef0(int enabled);
extern int func_001ada74(const struct SelectionOwner *owner);
extern void func_001adac8(struct SelectionOwner *owner, int selected_index);
extern unsigned short func_001c5b40(const unsigned short **cursor);
extern unsigned char func_001c5b54(const unsigned char **cursor);
extern unsigned char *func_001cfd40(const struct ModeTableState *state);
extern int func_001d3150(const struct ExtendedState *state);
extern void func_001d3d30(struct StreamRange *range, void *buffer, unsigned size);
extern void func_001d9090(struct SoundTrackPool *pool, void *resource);
extern struct SoundTrack *func_001d9198(unsigned first, unsigned second,
                                     unsigned third, unsigned fourth);
#ifdef __cplusplus
}
#endif

#endif
