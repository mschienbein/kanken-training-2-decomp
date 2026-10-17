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

/* INTEGRATOR: move to include/game/archive_stream.h */
extern "C" {
extern const unsigned int data_002244d4[];
ArchiveStreamObject *func_00146f44(ArchiveStreamObject *object, int enabled);
}

extern "C" ArchiveStreamObject *func_001436f4(ArchiveStreamObject *object)
{
    object = func_00146f44(object, 0);
    object->vtable = data_002244d4;
    return object;
}
