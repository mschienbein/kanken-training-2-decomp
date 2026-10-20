#include "game/stream_worker.h"

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
