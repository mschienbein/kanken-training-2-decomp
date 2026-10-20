#ifndef GAME_ARCHIVE_STREAM_H
#define GAME_ARCHIVE_STREAM_H

/* [INFERENCE] Observed embedded archive-stream lifecycle prefix only;
 * different resource/stream owners are not aliases of this object. */
struct ArchiveStreamContext { unsigned char reserved00[0x74]; };
struct ArchiveStreamObject {
    const void *vtable;
    unsigned char reserved04[0x118];
    struct ArchiveStreamContext context;
    unsigned char open, mode;
    unsigned char reserved192[2];
};
#ifdef __cplusplus
extern "C" {
#endif
extern const unsigned data_002244d4[];
extern struct ArchiveStreamObject *func_00146f44(struct ArchiveStreamObject *object, int enabled);
extern struct ArchiveStreamObject *func_001436f4(struct ArchiveStreamObject *object);
#ifdef __cplusplus
}
#endif

#endif
