#include "game/service_protocol.h"
#include "ctr/svc.h"

/* [INFERENCE] The adjacent publication words form one value object. */
struct PublicationFlags {
    unsigned int pending;
    unsigned int active;
    PublicationFlags(unsigned int pending_value, unsigned int active_value)
        : pending(pending_value), active(active_value) {}
};

struct GameResourceState {
    unsigned char reserved00[0x2e8];
    unsigned int value;
    struct PublicationFlags flags;
    void publish(unsigned int v) { value = v; flags = PublicationFlags(0, 1); }
};


struct ResourceTransferDescriptor {
    void *buffer;
    unsigned char kind;
    unsigned char direction;
    unsigned char mode;
    unsigned char reserved07;
};

/* INTEGRATOR: differs from src/wave1/func_0019e02c_returning.c: refines the
 * opaque initial 0x44 bytes and extends the evidenced handle field at +0x4dc. */
struct ResourceSet {
    unsigned char state;
    unsigned char transferred;
    unsigned char reserved02[0x0a];
    struct ResourceTransferDescriptor primary;
    struct ResourceTransferDescriptor transfers[1];
    unsigned char reserved1c[0x0c];
    struct ServiceObject temporary;
    struct ServiceObject service;
    unsigned char reserved30[0x10];
    struct ServiceObject secondary;
    void *first;
    void *second;
    void *allocation;
    unsigned char reserved50[0x48c];
    CtrHandle completion;
};

/* INTEGRATOR: move to include/game/resource_publication.h */
extern "C" {
extern struct GameResourceState bss_00296758;
extern struct ResourceSet bss_00291758;
}

extern "C" void func_0019ebd0(unsigned int value)
{
    struct GameResourceState *state = &bss_00296758;
    CtrHandle *handle = &bss_00291758.completion;
    state->publish(value);
    if (*handle != 0) {
        svcCloseHandle(*handle);
        *handle = 0;
    }
}
