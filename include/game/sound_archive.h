#ifndef GAME_SOUND_ARCHIVE_H
#define GAME_SOUND_ARCHIVE_H

struct SoundArchiveAccessors;
struct SoundArchiveAdapter;
struct SoundArchiveHook;
struct SoundArchiveReader;
struct SoundArchiveSupplement;
struct SoundRangeInfo;

#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
#ifdef __cplusplus
class SoundArchiveSupplement {
public:
    virtual void reserved00();
    virtual void reserved01();
    virtual void reserved02();
    virtual void reserved03();
    virtual void reserved04();
    virtual void reserved05();
    virtual void reserved06();
    virtual void reserved07();
    virtual void reserved08();
    virtual int readRange(unsigned item_id, SoundRangeInfo *output);

    int readRangeIfEnabled(unsigned item_id, SoundRangeInfo *output)
    {
        if (enabled == 0)
            return 0;
        return readRange(item_id, output);
    }

    unsigned char enabled;
    unsigned char reserved05_bytes[3];
};
#endif

#ifdef __cplusplus
struct SoundArchiveAdapter {
    const void *vtable;
    const SoundArchiveReader *reader;
    SoundArchiveSupplement *supplement;

    int readRange(unsigned item_id, SoundRangeInfo *output) const;
};
#endif

#ifdef __cplusplus
class SoundArchiveHook {
public:
    virtual void reserved00();
    virtual void reserved01();
    virtual void reserved02();
    virtual const void *get_sound_file(unsigned sound_id);
    virtual void reserved04();
    virtual void reserved05();
    virtual const void *get_external_file(const void *sound,
                                           const char *group, const char *name,
                                           unsigned bank_index);
    virtual int read_bank_file_id(unsigned bank_id, unsigned *file_id);
    int readBankFileIdIfEnabled(unsigned bank_id, unsigned *file_id)
    {
        if (enabled == 0)
            return 0;
        return read_bank_file_id(bank_id, file_id);
    }
    unsigned char enabled;
};
#endif

#ifdef __cplusplus
struct SoundArchiveAccessors {
    unsigned char reserved00[4];
    const SoundArchiveReader *archive;
    SoundArchiveHook *hook;
    int readBankFileId(unsigned bank_id, unsigned *file_id) const;
};
#endif

#ifdef __cplusplus
extern "C" {
#endif
int func_0013931c(const struct SoundArchiveReader *reader, unsigned item_id,
                  struct SoundRangeInfo *output);
int func_00139214(const struct SoundArchiveReader *archive, unsigned bank_id,
                   unsigned *file_id);
#ifdef __cplusplus
}
#endif

#endif
