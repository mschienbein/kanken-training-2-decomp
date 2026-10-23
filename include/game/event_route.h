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
union SecondaryCommandBuffer;
union VertexCommandBuffer;

#include <stdbool.h>
#include "game/archive.h"
#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct CommandVector { float x, y, z, w; };

union VertexCommandBuffer {
    struct { unsigned prefix[2], first_coefficient, header, payload[104]; } packet;
    unsigned words[108];
};

union SecondaryCommandBuffer {
    struct { unsigned prefix[2], first_coefficient, header, payload[128]; } packet;
    unsigned words[132];
};

struct DrawCommandState {
    unsigned reserved00;
    const unsigned *setup_commands;
    unsigned setup_size;
    unsigned char reserved0c[8];
    const unsigned *cached_commands;
    unsigned cached_size;
    unsigned char reserved1c[8];
    unsigned char vertex_count, row_count, secondary_count;
    unsigned char reserved27[0x61];
    union VertexCommandBuffer vertex_commands;
    unsigned char reserved238[0x210];
    union SecondaryCommandBuffer secondary_commands;
    struct CommandVector current;
    struct CommandVector *vertices;
    unsigned reserved66c;
    struct CommandVector *secondary;
    unsigned char reserved674[0xc];
    unsigned vertex_offset;
    unsigned active;
    unsigned char reserved688[0xc];
    unsigned mode;
    unsigned field698;
    int secondary_components;
    unsigned char flags, reserved6a1, kind;
};

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

/* Serialized byte-parameter records returned by the typed table lookup. */
struct BinaryParameterReader {
    unsigned reserved00;
    const struct BinaryTableBlock *block;
};
struct BinaryByteParameters {
    unsigned char channels[4];
    unsigned reserved04;
    int values_offset;
};
struct ByteParameterValue {
    unsigned char channels[4];
    unsigned char count;
    unsigned char values[2];
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
extern unsigned *func_0016b42c(struct DrawCommandState *state, unsigned *commands);
extern unsigned *func_0016b27c(struct DrawCommandState *state, unsigned *commands, const struct DrawTextureBinding *binding, unsigned reset);
extern unsigned *func_001dad48(struct DrawCommandState *state, unsigned *commands, unsigned flags);
extern const unsigned data_0021c2d8[];
bool func_001f261c(struct EventRoute *route, struct RoutedEvent *event);
extern unsigned *func_00165684(struct DrawCommandState *state, unsigned *commands);
extern unsigned *func_001dacf8(struct DrawCommandState *state, unsigned *commands, unsigned flags);
extern const struct BinaryReferenceTable *func_001f1f64(const struct BinaryTableBlock *block);
extern const struct BinaryByteParameters *func_001f1f80(const struct BinaryReferenceTable *table, unsigned index);
extern int func_001f29b4(const struct BinaryParameterReader *reader,
    struct ByteParameterValue *output, int index);
extern const void *func_001f22c4(const struct W12cSoundDetail *block);
extern const void *func_001f239c(const struct BankInstrumentInfo *block);
extern bool func_001f2a58(const struct BinaryVersionReader *reader);
extern const unsigned *func_001f494c(const struct DrawCommandState *state);
extern unsigned func_001f4958(const struct DrawCommandState *state, unsigned count);
#ifdef __cplusplus
}
#endif

#endif
