#ifndef GAME_INPUT_DEVICE_H
#define GAME_INPUT_DEVICE_H

struct AlignmentHandle;
struct DirtyValueObject;
struct DisplayBufferState;
struct InputBufferState;
struct InputCoordinates;
struct InputDeviceState;
struct InputStorageVTable;
struct InputStorageBase;
struct DrawCommandState;
struct InputStoragePointers;
struct PackedPosition;
struct PoolList;
struct ResetStateData;
struct ResetStateOwner;
struct ThreadStartContext;

#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct ResetStateData {
    unsigned reserved00;
    unsigned short value;
    unsigned char reserved06[2];
    unsigned char enabled;
};

struct ResetStateOwner {
    unsigned char reserved00[0x44];
    struct ResetStateData *state;
};

struct InputBufferState {
#ifdef __cplusplus
    InputBufferState();
#endif
    unsigned char *storage;
    unsigned count, capacity;
    unsigned char enabled;
    unsigned char reserved0d[3];
};

struct InputStorageBuffer {
    unsigned word00, word04, word08;
    unsigned char enabled;
    unsigned char reserved0d[3];
};

struct InputStoragePointers {
    unsigned char *first;
    unsigned char *second;
    unsigned char *third;
    unsigned char *fourth;
};

struct InputCoordinates {
    float first, second, third, fourth;
};

struct CommandVector { float x, y, z, w; };

/* This distinct allocation is0x688 bytes; its ready byte684 is not the
 * active word of the six-word draw prefix. Constructor0010cc0c installs223d4c. */
struct InputStorageBase {
#ifdef __cplusplus
    InputStorageBase();
#endif
    const struct InputStorageVTable *vtable;
    struct InputStorageBuffer first, second;
    unsigned char flags[4];
    unsigned char reserved28[0x94 - 0x28];
    unsigned char first_storage[0x244 - 0x94];
    unsigned char second_storage[0x284 - 0x244];
    unsigned char fourth_storage[0x454 - 0x284];
    unsigned char third_storage[0x658 - 0x454];
    struct InputCoordinates coordinates;
    struct InputStoragePointers storage;
    unsigned word678, word67c, word680;
    unsigned char ready;
    unsigned char reserved685[3];
};

#ifdef __cplusplus
/* [INFERENCE] Native hierarchy/method names represent the prefix constructed
 * by00116eb0 inside0010e2c8's0x6a4 allocation. The original tables have
 * asymmetric null destructor cells, plausibly virtual-function elimination;
 * no absent destructor is bound or invoked. */
struct InputDeviceState {
    InputDeviceState();
    virtual ~InputDeviceState();
    virtual void reset();
    virtual unsigned char *commit(unsigned flags);
    virtual unsigned char *commit_to(unsigned char *cursor, unsigned flags);
    virtual unsigned char *reserve(unsigned flags);
    virtual unsigned char *reserve_to(unsigned char *cursor, unsigned flags);
    InputBufferState setup;
    InputBufferState cached;
    unsigned char vertex_count, row_count, secondary_count, reserved27;
    unsigned char reserved28[0x88 - 0x28];
    unsigned char vertex_commands[0x1b0];
    unsigned char reserved238[0x210];
    unsigned char secondary_commands[0x210];
    CommandVector current;
    unsigned char *vertices_storage, *second_storage;
    unsigned char *secondary_storage, *fourth_storage;
    unsigned char *buffer;
    unsigned capacity, vertex_offset, active, word688, word68c;
    unsigned char ready;
    unsigned char reserved691[3];
};
#endif

struct ThreadStartContext {
    void (*cleanup)(void *context);
    void (*entry)(void *argument, void *context);
    void *argument;
    void *context;
    void *termination_argument;
};

struct DisplayBufferState {
    void *storage;
    unsigned count;
    unsigned capacity;
    unsigned char enabled;
    unsigned char reserved0d[3];
};

struct PackedPosition {
    short x;
    short y;
};

struct DirtyValueObject {
    unsigned char reserved00[0x10];
    struct PackedPosition value;
    unsigned char reserved14[0x58];
    unsigned short dirty;
};



#ifdef __cplusplus
extern "C" {
#endif
extern void func_00121364(void);
extern void func_001213b0(void);
extern void func_0012d0b8(void);
extern void func_0012d0c4(void *argument);
extern void func_0012d148(void);
extern void func_0010c480(struct ResetStateOwner *owner);
extern struct PoolList *func_0011ee7c(struct PoolList *list);
extern struct PoolList *func_0011ee94(struct PoolList *list);
extern struct PoolList *func_0011eeac(struct PoolList *list);
extern struct InputBufferState *func_00123b38(struct InputBufferState *buffer);
extern struct InputStorageBuffer *func_001149a8(struct InputStorageBuffer *buffer);
extern const struct InputStorageVTable data_00223d4c;
extern void func_001c5b68(struct InputStorageBase *object);
extern void func_0012d104(struct ThreadStartContext *start);
extern struct PoolList *func_001342f4(struct PoolList *list);
extern struct PoolList *func_00144970(struct PoolList *list);
extern struct DisplayBufferState *func_0014ee44(struct DisplayBufferState *buffer);
extern void func_00152624(struct DirtyValueObject *object, const struct PackedPosition *value);
#ifdef __cplusplus
}
#endif

#endif
