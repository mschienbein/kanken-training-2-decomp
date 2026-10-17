#ifndef GAME_FILE_STREAM_H
#define GAME_FILE_STREAM_H

struct FileHandleObject;
struct Result;
struct FileStreamView;



#include "game/result.h"
#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
#ifdef __cplusplus
struct FileHandleObject {
    virtual void reserved0();
    virtual void reserved1();
    virtual void reserved2();
    virtual void reserved3();
    virtual void reserved4();
    virtual void reserved5();
    /* [INFERENCE] The factory maps priority classes to -16, -8 and 8. */
    virtual Result set_priority(int priority);
};
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern struct Result func_001c4af0(struct FileHandleObject *file, int priority);
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
#include "game/resource_stream.h"
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */


struct FileStreamView {
    const void *vtable;
    ResourceStream state;
};

extern "C" {
int func_00180578(ResourceStream *state, long long position);
}
#endif

#endif
