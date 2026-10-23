#include "game/render_state.h"


/* [INFERENCE] Device and host release helpers are shared by buffer-mode paths. */
static void release_device_memory(struct W6eGpuBuffer *object,
                                  struct W6eGpuBufferPayload *buffer, unsigned kind);
static void release_host_memory(struct W6eGpuBuffer *object,
                                struct W6eGpuBufferPayload *buffer, unsigned kind);

void func_0018c784(struct W6eGpuBuffer *object)
{
    struct W6eGpuBufferPayload *buffer = object->payload.buffer;
    switch (buffer->mode) {
    case 0x2010000:
        break;
    case 0x2020000:
    case 0x2030000:
        release_device_memory(object, buffer, 0x102);
        break;
    case 0x1010000:
        release_host_memory(object, buffer, 0x102);
        break;
    case 0x1020000:
    case 0x1030000:
        release_device_memory(object, buffer, 0x102);
        release_host_memory(object, buffer, 0x102);
        break;
    }
    buffer->device_memory = 0;
    buffer->host_memory = 0;
}

static void release_device_memory(struct W6eGpuBuffer *object,
                                  struct W6eGpuBufferPayload *buffer, unsigned kind)
{
    void (*release)(unsigned, unsigned, unsigned, void *) = data_002429f8;
    if (buffer->device_memory && release)
        release(buffer->module, kind, object->handle, buffer->device_memory);
}

static void release_host_memory(struct W6eGpuBuffer *object,
                                struct W6eGpuBufferPayload *buffer, unsigned kind)
{
    if (buffer->host_memory) {
        void (*release)(unsigned, unsigned, unsigned, void *) = data_002429f8;
        if (release)
            release(0x10000, kind, object->handle, buffer->host_memory);
    }
}
