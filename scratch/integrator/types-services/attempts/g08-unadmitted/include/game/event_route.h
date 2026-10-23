#ifndef GAME_EVENT_ROUTE_H
#define GAME_EVENT_ROUTE_H

struct ArchiveFile;
struct BinaryByteParameters;
struct W12cSoundDetail;
struct BinaryReferenceTable;
struct BinaryTableBlock;
struct BinaryVersionReader;
struct CommandVector;
struct DrawCommandState;
struct DrawTextureBinding;
struct EventDispatcher;
struct EventReceiver;
struct EventRoute;
struct PrimaryEventDispatcher;
struct RelativeReference;
struct RoutedEvent;

#include <stdbool.h>
#include "game/archive.h"
#include "game/runtime.h"
#include "game/input_device.h"

/* [INFERENCE] The six-word construction prefix is a real native base; the
 * derived suffix and installed table are proved by0010e2c8 and001dacf8. */
#ifdef __cplusplus
struct DrawCommandState : InputDeviceState {
    DrawCommandState();
    virtual ~DrawCommandState();
    virtual void reset();
    virtual unsigned char *commit(unsigned flags);
    virtual unsigned char *commit_to(unsigned char *cursor, unsigned flags);
    virtual unsigned char *reserve(unsigned flags);
    virtual unsigned char *reserve_to(unsigned char *cursor, unsigned flags);
    unsigned mode;
    unsigned field698;
    int secondary_components;
    unsigned char flags, reserved6a1, kind;
    unsigned char reserved6a3;
};
#endif

struct BinaryReferenceTable {
    int count;
#ifndef __cplusplus
    struct RelativeReference entries[];
#endif
};

struct BinaryTableBlock {
    unsigned signature;
    unsigned size;
    struct RelativeReference table;
};


struct BinaryVersionReader {
    const struct ArchiveFile *file;
};

#ifdef __cplusplus
class EventReceiver {
public:
    virtual void slot00();
    virtual void slot01();
    virtual bool receive(RoutedEvent *event);
};
#endif

#ifdef __cplusplus
class EventRoute {
public:
    virtual void slot00();
    EventReceiver *receiver;
    void dispatch(RoutedEvent *event);
};
#endif

#ifdef __cplusplus
class PrimaryEventDispatcher {
public:
    virtual void slot00();
    virtual void slot01();
    virtual void slot02();
    virtual void slot03();
    virtual void receive(RoutedEvent *event);
    unsigned char reserved04[8];
};
#endif

#ifdef __cplusplus
class EventDispatcher : public PrimaryEventDispatcher, public EventRoute {
public:
    unsigned char reserved14[0x208];
    EventReceiver *priority_receiver;
    void dispatch(RoutedEvent *event);
    void deliver(RoutedEvent *event);
};
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern unsigned char *func_0016b42c(struct DrawCommandState *state, unsigned char *commands);
extern unsigned char *func_0016b27c(struct DrawCommandState *state, unsigned char *commands, const struct DrawTextureBinding *binding, unsigned reset);
extern const unsigned data_0021c2d8[];
bool func_001f261c(struct EventRoute *route, struct RoutedEvent *event);
extern unsigned char *func_00165684(struct DrawCommandState *state, unsigned char *commands);
extern const struct BinaryReferenceTable *func_001f1f64(const struct BinaryTableBlock *block);
extern const struct BinaryByteParameters *func_001f1f80(const struct BinaryReferenceTable *table, unsigned index);
extern const void *func_001f22c4(const struct W12cSoundDetail *block);
extern const void *func_001f239c(const struct BankInstrumentInfo *block);
extern bool func_001f2a58(const struct BinaryVersionReader *reader);
extern const unsigned *func_001f494c(const struct DrawCommandState *state);
extern unsigned func_001f4958(const struct DrawCommandState *state, unsigned count);
#ifdef __cplusplus
}
#endif

#endif
