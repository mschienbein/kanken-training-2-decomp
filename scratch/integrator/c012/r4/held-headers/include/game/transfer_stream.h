#ifndef GAME_TRANSFER_STREAM_H
#define GAME_TRANSFER_STREAM_H
#include "game/result.h"
#ifdef __cplusplus
/* np134's exact seek/read caller supplies these two typed virtual slots.
 * Other slot roles and descriptive stream identity remain [INFERENCE]; this
 * is not an alias of the separate byte-count-returning archive FileStream. */
class TransferStream {
public:
    virtual void reserved00();
    virtual void reserved01();
    virtual void reserved02();
    virtual void reserved03();
    virtual void reserved04();
    virtual void reserved05();
    virtual Result seek(long long offset);
    virtual void reserved07();
    virtual void reserved08();
    virtual void reserved09();
    virtual Result read(unsigned *transferred, void *buffer, unsigned size);
};

extern "C" unsigned func_0019000c(TransferStream *stream, unsigned char *buffer,
    int position, int displacement, unsigned size);
#endif
#endif
