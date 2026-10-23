#include "game/runtime.h"
#include "game/synchronization.h"

static void copy_request_bytes(unsigned char *destination, const unsigned char *source, int count)
{
    int index;
    for (index = 0; index < count; ++index)
        destination[index] = source[index];
}

/* [INFERENCE] Thread entry erases its captured pointer; the object has the
 * canonical semaphore/signal union views used by initialization and waiting.
 * The worker clears active only after draining the queue; stopping leaves it
 * unchanged, as in the original. */
void func_0010aa28(void *argument)
{
    struct SignalState *request = argument;
wait_for_request:
    func_00195d94(request);
    if (bss_002d4270.stop != 0)
        return;
next_request:
    if (bss_002d4270.write == bss_002d4270.read)
        goto idle;
    {
        struct CopyRequest *copy = &bss_002d4270.requests[bss_002d4270.read];
        unsigned char *destination = copy->destination;
        const unsigned char *source = copy->source;
        unsigned count = copy->count;

        if (source[0] == 'E' && source[1] == 'C' && source[2] == 'D') {
            count = (source[13] << 16) + (source[14] << 8) + source[15];
            if (source[3] == 0) {
                source += 16;
            } else {
                func_00192c20(source, bss_002d4270.buffer);
                source = bss_002d4270.buffer;
            }
        }
        copy_request_bytes(destination, source, (int)count);
        copy->destination = 0;
        copy->source = 0;
        copy->count = 0;
        bss_002d4270.read = (bss_002d4270.read + 1) % 256;
    }
    goto next_request;
idle:
    bss_002d4270.active = 0;
    goto wait_for_request;
}
