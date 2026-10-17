#include "game/result.h"

struct ResourceResult {
    Result code;
    unsigned detail;

    ResourceResult();
};

ResourceResult::ResourceResult() : code(0xe7e3ffffu), detail(0)
{
}
