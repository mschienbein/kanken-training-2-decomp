#ifndef GAME_OBJECT_POOL_H
#define GAME_OBJECT_POOL_H

struct CalendarState;
struct DateTime;
struct DisplayHandle;
struct NameTable;
struct ObjectPool;
struct PoolLink;
struct PoolList;
struct SizedBuffer;
struct SizedBufferHandle;

#include "game/pool_list.h"
#include "game/runtime.h"
#include "game/property_descriptor.h"
#include "game/sound_runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct ObjectPool {
    struct FreeListPool free_list;
    struct PoolList active;
    unsigned char initialized;
    unsigned char reserved11[3];
    unsigned int capacity;
    void *storage;
    unsigned int storage_size;
};

struct SizedBuffer {
    unsigned int reserved00[2];
    unsigned int size;
};

struct SizedBufferHandle {
    unsigned int reserved00;
    const struct SizedBuffer *buffer;
};

#ifdef __cplusplus
extern "C" {
#endif
extern volatile unsigned data_00243718;
extern struct ObjectPool bss_004b8420;
extern struct ObjectPool *func_00185cc0(void);
extern void func_00147264(struct ObjectPool *pool, void *storage, unsigned size);
extern int func_0019107c(const struct DateTime *time);
extern int func_00191024(const struct DateTime *time);
extern int func_00190fcc(const struct DateTime *time);
extern unsigned char func_001efb68(const struct DateTime *time);
extern int func_001efb00(const struct DateTime *time);
extern void func_0018d074(int *year_out, int *month_out, int *day_out, int days);
extern void func_001c229c(struct CalendarState *calendar, int mode, int year, int month);
extern unsigned int func_00181384(const struct ObjectPool *pool, unsigned int count);
extern struct PoolLink *func_00191144(struct ObjectPool *pool, struct PoolLink *node);
extern unsigned int func_001932d8(const struct SizedBufferHandle *handle);
extern void func_001c1bf0(struct DisplayHandle **handles, int index, int value);
extern void func_001c228c(struct CalendarState *calendar, int year, int month);
#ifdef __cplusplus
}
#endif

#endif
