#ifndef GAME_SOUND_CONTROLLER_H
#define GAME_SOUND_CONTROLLER_H

struct AudioTaskManager;
struct FileSoundController001d40a8;
struct ForwardedFileStream;
struct ForwardingFileStream;
struct InputSessionContext;
struct ResetState001d4850;
struct ResourceFontView;
struct ResourceOwner;
struct ResourceState001d0164;
struct StreamCriticalSection;
struct StreamDecodeState;
struct StreamMutex;
struct X03PendingCommand;

#include <stdbool.h>
#include "game/audio_voice.h"
#include "game/resource_owner.h"
#include "game/runtime.h"
#include "game/stream_manager.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct X03PendingCommand {
    unsigned words[128];
};


struct StreamDecodeState {
    unsigned char reserved00[0x38];
    unsigned first;
    unsigned second;
    unsigned third;
};

#ifdef __cplusplus
class ForwardedFileStream {
public:
    typedef void (*Callback)(int, ForwardedFileStream *, void *);
    virtual const void *runtime_type() const;
    virtual ~ForwardedFileStream();
    virtual bool CanRead() const;
    virtual bool CanWrite() const;
    virtual bool CanAsync() const;
    virtual unsigned GetOffsetAlign() const;
    virtual unsigned GetSizeAlign() const;
    virtual unsigned GetBufferAlign() const;
    virtual int Read(void *buffer, unsigned length);
    virtual bool ReadAsync(void *buffer, unsigned length, Callback callback, void *argument);
    virtual int Write(const void *buffer, unsigned length);
    virtual bool WriteAsync(const void *buffer, unsigned length, Callback callback, void *argument);
    virtual bool IsBusy() const;
    virtual void Close();
    virtual unsigned GetSize() const;
    virtual void Seek(int offset, unsigned origin);
    virtual void Cancel();
    virtual bool CancelAsync(Callback callback, void *argument);
    virtual bool CanSeek() const;
};
#endif

struct ForwardingFileStream {
    unsigned char reserved00[0x14];
    struct ForwardedFileStream *stream;
};

struct ResetState001d4850 {
    unsigned char reserved00[0x38];
    void *first;
    void *second;
    void *third;
};

struct FileSoundController001d40a8 {
    struct ResourceState001d0164 base;
    unsigned char reserved10[0x10c];
    struct ResourceOwner owner;
    unsigned char reserved124[0x18];
    struct ResetState001d4850 reset;
    unsigned char reserved180[0x0c];
    int index;
    unsigned char active;
};


#ifdef __cplusplus
extern "C" {
#endif
extern int func_001362b4(unsigned *detail);
extern unsigned func_001362f0(struct X03PendingCommand *command);
extern void func_0012e560(struct StreamCriticalSection *critical);
extern struct FileSoundController001d40a8 *func_001d40a8(struct FileSoundController001d40a8 *state);
extern const unsigned data_002247f0[];
extern void func_001d2da4(struct ResourceOwner *owner);
extern struct ResourceState001d0164 *func_001d017c(struct ResourceState001d0164 *state);
extern unsigned func_0012d40c(unsigned *detail);
extern unsigned func_0012d424(struct X03PendingCommand *command);
extern void func_0014856c(struct InputSessionContext *object);
extern struct StreamDecodeState *func_00149374(struct StreamDecodeState *state);
extern void func_00180860(struct AudioTaskManager *manager);
extern bool func_001f1fe0(const struct ForwardingFileStream *object);
extern bool func_001f1ff0(const struct ForwardingFileStream *object);
extern unsigned func_001f2000(const struct ForwardingFileStream *object);
extern int func_001f4b2c(const struct ResourceFontView *font);
extern int func_001f4b48(const struct ResourceFontView *font);
extern bool func_001f4cb4(const struct ResourceFontView *font);
extern bool func_001f4cc4(const struct ResourceFontView *font);
extern void func_001d01cc(struct FileSoundController001d40a8 *state);
#ifdef __cplusplus
}
#endif

#endif
