#ifndef GAME_MEMORY_H
#define GAME_MEMORY_H

struct PackedRecordSource;
struct PacketStream;

#include <stdbool.h>
#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
#ifdef __cplusplus
class PacketStream {
public:
    virtual void slot00();
    virtual void slot01();
    virtual void slot02();
    virtual void slot03();
    virtual void slot04();
    virtual void slot05();
    virtual void slot06();
    virtual void slot07();
    virtual void slot08();
    virtual unsigned int read(void *buffer, unsigned int size);
    virtual void slot10();
    virtual void slot11();
    virtual void slot12();
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
    virtual void seek(unsigned int position, int origin);
};
#endif

struct PackedRecordSource {
    struct PacketStream *stream;
    unsigned int unknown04;
    unsigned int base;
    unsigned short stride;
};

#ifdef __cplusplus
extern "C" {
#endif
extern bool func_001f281c(struct PackedRecordSource *source, void *output, unsigned int index);
#ifdef __cplusplus
}
#endif

#endif
