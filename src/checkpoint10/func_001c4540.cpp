#include "ctr/svc.h"

struct FileStreamState {
    unsigned int flags;
    unsigned int offset_low;
    unsigned int offset_high;
};
struct FileStreamBridge {
    const void *vtable;
    struct FileStreamState state;
};
/* INTEGRATOR: move to include/game/file_stream.h */
extern "C" {
extern CtrResult func_001c5074(struct FileStreamState *stream, unsigned int *written,
                              const void *buffer, unsigned int size, int flush);
}

extern "C" CtrResult func_001c4540(struct FileStreamBridge *stream, unsigned int *written,
                       const void *buffer, unsigned int size, int flush)
{
    return func_001c5074(&stream->state, written, buffer, size, flush);
}
