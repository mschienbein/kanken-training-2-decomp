#ifndef GAME_INPUT_DEVICE_H
#define GAME_INPUT_DEVICE_H

struct AlignmentHandle;
struct DirtyValueObject;
struct DisplayBufferState;
struct InputBufferState;
struct InputCoordinates;
struct InputDeviceState;
struct InputDeviceVTable;
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

struct InputStoragePointers {
    unsigned char *first;
    unsigned char *second;
    unsigned char *third;
    unsigned char *fourth;
};

struct InputCoordinates {
    float first, second, third, fourth;
};

#ifdef __cplusplus
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
extern void func_0012d104(struct ThreadStartContext *start);
extern struct PoolList *func_001342f4(struct PoolList *list);
extern struct PoolList *func_00144970(struct PoolList *list);
extern struct DisplayBufferState *func_0014ee44(struct DisplayBufferState *buffer);
extern void func_00152624(struct DirtyValueObject *object, const struct PackedPosition *value);
#ifdef __cplusplus
}
#endif

#endif
