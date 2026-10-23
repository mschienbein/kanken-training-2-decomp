#include "game/aux_effect.h"
/* [INFERENCE] Field roles in this 12-byte vector-constructor record are unrecovered. */

void *func_0014ee30(void *storage)
{
    struct AudioRecord12 *record = storage;
    record->word00 = 0;
    record->word04 = 0;
    record->active = 0;
    return record;
}
