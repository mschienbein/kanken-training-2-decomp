#include "game/result.h"

struct ResourceCursorState {
    const void *record;
    unsigned reserved04;
    Result status;
    unsigned char reserved0c[8];

    ResourceCursorState();
};

ResourceCursorState::ResourceCursorState() : status(~0u)
{
}
