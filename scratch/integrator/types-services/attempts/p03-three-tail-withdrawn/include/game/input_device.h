#ifndef GAME_INPUT_DEVICE_H
#define GAME_INPUT_DEVICE_H

struct AlignmentHandle;
struct DirtyValueObject;
struct DisplayBufferState;
struct InputBufferState;
struct InputCoordinates;
struct InputDeviceState;
struct InputDeviceVTable;
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
    void *storage;
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


/* Constructor0010cc0c and its allocation prove this distinct0x688 owner.
 * No native relationship to the six-word drawer family is asserted. */
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
/* The six-word constructor/native drawer relationship remains deferred; keep
 * this existing opaque construction interface until its native base is proved. */
struct InputDeviceState {
    InputDeviceState();
    const InputDeviceVTable *vtable;
    InputBufferState first;
    InputBufferState second;
    unsigned char flags[4];
    unsigned char reserved28[0x94 - 0x28];
    unsigned char first_storage[0x244 - 0x94];
    unsigned char second_storage[0x284 - 0x244];
    unsigned char fourth_storage[0x454 - 0x284];
    unsigned char third_storage[0x658 - 0x454];
    InputCoordinates coordinates;
    InputStoragePointers storage;
    unsigned counts[6];
    unsigned char ready;
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
extern const struct InputDeviceVTable data_002241f8;
struct InputDeviceState *func_00116eb0(struct InputDeviceState *object);
extern struct InputDeviceState *func_0010e2c8(struct InputDeviceState *object);
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
