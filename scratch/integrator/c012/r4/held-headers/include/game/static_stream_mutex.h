#ifndef GAME_STATIC_STREAM_MUTEX_H
#define GAME_STATIC_STREAM_MUTEX_H

#include "game/stream_manager.h"
#include "game/object_manager.h"

/* The three names-admission initializers establish a mutex-sized owner with
 * its mutex at offset zero. C++ uses the ordinary base conversion; C names
 * the same prefix as value. Neither view allocates a second mutex or casts
 * between competing globals. Historical class spelling remains [INFERENCE]. */
#ifdef __cplusplus
struct StaticStreamMutex : StreamMutex {
    StaticStreamMutex()
    {
        critical.state = 0;
        func_00195ec4(this);
    }
    ~StaticStreamMutex();
};
#else
struct StaticStreamMutex {
    struct StreamMutex value;
};
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern struct StaticStreamMutex bss_0049ee70;
extern struct StaticStreamMutex bss_0049eeb8;
extern struct StaticStreamMutex bss_004b4fb4;
#ifdef __cplusplus
}
#endif

#endif
