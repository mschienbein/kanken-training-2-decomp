#include "ctr/cp15.h"
#include "game/stream_manager.h"

struct X07PublicationEvent {
    unsigned int state;
    struct StreamCriticalSection critical;
};

struct SharedPublicationState {
    signed char current;
    signed char published;
    unsigned char pending;
    unsigned char ready;
    signed char shared_status;
    unsigned char reserved[3];
    struct X07PublicationEvent event;
};

extern struct SharedPublicationState data_00242d90;
extern void func_0019ea6c(struct X07PublicationEvent *event);

void func_0013e810(void)
{
    if ((data_00242d90.current & data_00242d90.published) != 0) {
        data_00242d90.ready = 1;
        data_00242d90.published = 0;
        data_00242d90.pending = 0;
        ctrDataSynchronizationBarrier();
        func_0019ea6c(&data_00242d90.event);
    }
}
