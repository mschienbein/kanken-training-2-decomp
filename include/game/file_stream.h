#ifndef GAME_FILE_STREAM_H
#define GAME_FILE_STREAM_H

struct FileHandleObject;
struct Result;
struct FileStreamView;



#include "game/result.h"
#include "game/runtime.h"
#include "ctr/svc.h"

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

/* [INFERENCE] This actual seek/write interface is distinct from FileHandleObject
 * and FileStreamView. Uncalled slots describe layout, not callable contracts. */
class PositionedOutputStream {
public:
    virtual void slot00();
    virtual void slot04();
    virtual void slot08();
    virtual void slot0c();
    virtual void slot10();
    virtual void slot14();
    virtual int seek(long long offset);
    virtual void slot1c();
    virtual void slot20();
    virtual void slot24();
    virtual int write(int *written, const void *buffer, unsigned size, bool flush);
};
extern "C" bool func_0018ff34(PositionedOutputStream *stream,
    const unsigned char *buffer, int position, int buffer_offset, unsigned size);
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern struct Result func_001c4af0(struct FileHandleObject *file, int priority);
#ifdef __cplusplus
}
#endif

/* [INFERENCE] Forwarded stream prefix is distinct from the resource-size API. */
struct FileStreamState { unsigned flags, offset_low, offset_high; };
struct FileStreamBridge {
    const void *vtable;
    struct FileStreamState state;
};
#ifdef __cplusplus
extern "C" {
#endif
extern CtrResult func_001c5074(struct FileStreamState *stream, unsigned *written,
                             const void *buffer, unsigned size, int flush);
extern CtrResult func_001c4540(struct FileStreamBridge *stream, unsigned *written,
                             const void *buffer, unsigned size, int flush);
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
