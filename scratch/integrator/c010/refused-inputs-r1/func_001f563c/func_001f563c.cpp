#include "game/stream_manager.h"
#include "game/object_manager.h"

struct StaticStreamMutex {
    StreamMutex value;
    StaticStreamMutex()
    {
        value.critical.state = 0;
        func_00195ec4(&value);
    }
    ~StaticStreamMutex();
};

StaticStreamMutex bss_0049ee70;
