#ifndef GAME_TASK_POOL_H
#define GAME_TASK_POOL_H

#include "game/pool_list.h"

/* nf057's drain and nf026's release agree with the accepted priority-release
 * client on active at +4. Together they establish the initialized byte and
 * second list at +0x10. This is a prefix, not an asserted allocation extent.
 * [INFERENCE] The second list's available role names the observed free group
 * list; both lists contain canonical AudioVoiceGroup.link nodes. */
struct TaskPool {
    unsigned char initialized;
    unsigned char reserved01[3];
    struct PoolList active;
    struct PoolList available;
};

#endif
