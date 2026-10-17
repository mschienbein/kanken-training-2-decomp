#include "game/synchronization.h"

struct W22Stream;
struct W22StreamVTable {
    const void *reserved00[3];
    void (*seek)(W22Stream *stream, long long position, int mode);
    const void *reserved10[7];
    void (*transfer)(W22Stream *stream, int destination, int size);
};
struct W22Stream { const W22StreamVTable *vtable; };
struct W22StreamJob {
    unsigned char reserved00[12];
    unsigned cancelled;
    W22Stream *stream;
    unsigned char reserved14[0x44];
    int destination, length, source;
};
struct W22StreamWorker {
    unsigned char reserved00[12];
    unsigned pending, stopping;
    unsigned char reserved14[0x1014];
    W22StreamJob *job;
};
/* INTEGRATOR: move to include/game/stream_worker.h */
extern "C" {
extern W22StreamWorker bss_004962a8;
void func_00107ce4(SignalState *wake);
}

void func_00107ce4(SignalState *wake)
{
    for (;;) {
        func_00195d94(wake);
        if (bss_004962a8.stopping)
            break;
        if (bss_004962a8.pending) {
            W22StreamJob *job = bss_004962a8.job;
            int whole_chunks = job->length / 65536;
            for (int i = 0; i <= whole_chunks; ++i) {
                if (job->cancelled)
                    break;
                int size = i == whole_chunks ? job->length - whole_chunks * 65536 : 65536;
                int offset = i * 65536;
                job->stream->vtable->seek(job->stream, static_cast<long long>(job->source + offset), 0);
                job->stream->vtable->transfer(job->stream, job->destination + offset, size);
            }
        }
        bss_004962a8.pending = 0;
    }
}
