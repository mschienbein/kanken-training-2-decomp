#ifndef GAME_DISPLAY_ANIMATION_H
#define GAME_DISPLAY_ANIMATION_H


struct ArchiveParseArena;
struct ArchiveParseContext;
struct AudioChannel;
struct AudioChannelPacket;
struct AudioChannelState;
struct HandleManager;
struct BoundDisplayObject;
struct DisplayAnimationController;
struct DisplayAnimationNode;
struct DisplayObject;
struct DisplayRegistration;
struct InputSessionContext;
struct MenuItem;
struct PositionableStream;
struct PositionableStreamVTable;
struct QuestionRecord;
struct QuestionTable;
struct Result;
struct ServiceOutput;
struct StreamMutex;

#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */

struct PositionableStreamVTable {
    unsigned int reserved00[4];
    struct Result (*seek)(struct PositionableStream *stream, long long offset);
};

struct PositionableStream {
    const struct PositionableStreamVTable *vtable;
};

struct AudioChannelPacket {
    unsigned short flags;
    unsigned short sequence;
    unsigned int position;
    unsigned short first;
    unsigned short last;
};


struct QuestionRecord {
    unsigned char reserved00[0x1f];
    signed char enabled;
    unsigned char reserved20[4];
};

struct QuestionTable {
    unsigned int reserved00[2];
    const struct QuestionRecord *records;
};


/* QUALIFIED legacy material-family payload; needs-canonical-cutover.
 * Canonical display code does not access or convert this accepted view. */
struct DisplayAnimationSlot {
    unsigned int in_use;
    int kind;
    unsigned int field08;
    float speed;
    unsigned char alpha;
    unsigned char reserved11[3];
    int field14;
    int field18;
    int field1c;
    int field20;
    int field24;
    int playback_state;
    int animation_id;
    unsigned int field30;
    struct DisplayAnimationController *controller;
    struct DisplayAnimationNode **reference;
};

struct DisplayRegistration {
    struct DisplayObject *object;
    int enabled;
};


#ifdef __cplusplus
extern "C" {
#endif
extern struct ArchiveParseArena bss_004b1000;
extern struct StreamMutex bss_004b4f6c;
extern void func_00146168(struct ServiceOutput *renderer, float volume);
extern void func_00154f94(struct AudioChannelState *state, const struct AudioChannelPacket *packet);
extern void func_001c1e6c(struct DisplayRegistration *const *slots, int index, int active);
extern struct ArchiveParseArena *func_0012d858(struct ArchiveParseContext *context);
extern struct Result func_0012e0c0(struct PositionableStream *stream, long long offset);
extern void func_0012fb3c(void);
extern void func_0012fb4c(void);
extern void func_00137e60(void);
extern int func_00137fe8(void);
extern void func_0014282c(float volume);
/* [INFERENCE] The owner is forwarded but unused. Preserve the full word: the
 * matching caller sign-extends a device configuration byte, including negatives. */
extern int func_001462d4(struct InputSessionContext *owner, int value);
extern void func_0015434c(struct ServiceOutput *renderer);
extern void func_00154f78(struct HandleManager *router, int index,
                   const struct AudioChannelPacket *packet);
extern int func_001672dc(const struct QuestionTable *table, int index);
extern void func_0016bf80(struct DisplayObject *object, int index, float speed);
extern void func_00173d90(struct DisplayRegistration *const *slots, int index, int active);
extern void func_00177740(const struct MenuItem *metadata, int *first, int *second);
extern void func_001810e4(struct BoundDisplayObject *object, int index);
#ifdef __cplusplus
}
#endif

#endif
