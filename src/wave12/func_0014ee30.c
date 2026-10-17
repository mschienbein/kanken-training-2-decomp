#include "game/aux_effect.h"
/* [INFERENCE] Field roles in this 12-byte vector-constructor record are unrecovered. */

struct AudioRecord12 *func_0014ee30(struct AudioRecord12 *record)
{
    record->word00 = 0;
    record->word04 = 0;
    record->active = 0;
    return record;
}
