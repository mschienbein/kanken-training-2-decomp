#include <new>

struct ArchiveStreamContext {
    unsigned char reserved00[0x74];
};
struct ArchiveStreamObject {
    const void *vtable;
    unsigned char reserved04[0x118];
    ArchiveStreamContext context;
    unsigned char open;
    unsigned char mode;
    unsigned char reserved192[2];
};
struct ArchiveTransfer {
    unsigned char reserved00[0x50];
    unsigned char valid;
    unsigned char reserved51[3];
    ArchiveTransfer(ArchiveStreamContext *context, int first_option, int second_option,
        void (*notify)(unsigned int, ArchiveStreamObject *), ArchiveStreamObject *object);
};

/* INTEGRATOR: move to include/game/archive_stream.h */
extern "C" {
void func_001d4078(unsigned int state, ArchiveStreamObject *object);
}

extern "C" ArchiveTransfer *func_001d4010(ArchiveStreamObject *object,
    ArchiveTransfer *storage, unsigned int size, int first_option, int second_option)
{
    if (object->open && size >= sizeof(ArchiveTransfer)) {
        ArchiveTransfer *transfer = new (storage) ArchiveTransfer(&object->context,
            first_option, second_option, func_001d4078, object);
        /* ORIGINAL BUG: a null storage pointer still reaches the validity read. */
        if (transfer->valid)
            return transfer;
    }
    return 0;
}
