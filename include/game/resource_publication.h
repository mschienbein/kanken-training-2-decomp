#ifndef GAME_RESOURCE_PUBLICATION_H
#define GAME_RESOURCE_PUBLICATION_H

#include "game/service_protocol.h"

/* [INFERENCE] Adjacent publication words form one value object. These are
 * compatible partial views of the exact globals, not complete native classes. */
struct PublicationFlags {
    unsigned pending;
    unsigned active;
#ifdef __cplusplus
    PublicationFlags(unsigned pending_value, unsigned active_value)
        : pending(pending_value), active(active_value) {}
#endif
};
struct GameResourceState {
    unsigned char reserved00[0x2e8];
    unsigned value;
    struct PublicationFlags flags;
    unsigned char reserved2f4[0x0c];
    void *resource;
#ifdef __cplusplus
    void publish(unsigned v) { value = v; flags = PublicationFlags(0, 1); }
#endif
};
struct ResourceTransferDescriptor {
    void *buffer;
    unsigned char kind, direction, mode, reserved07;
};
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

#ifdef __cplusplus
extern "C" {
#endif
extern struct GameResourceState bss_00296758;
extern struct ResourceSet bss_00291758;
extern void func_0019ebd0(unsigned value);
extern void func_0019e02c(void);
extern int func_001043c8(void);
#ifdef __cplusplus
}
#endif

#endif
