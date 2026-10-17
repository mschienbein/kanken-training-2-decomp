#ifndef GAME_SERVICE_RUNTIME_H
#define GAME_SERVICE_RUNTIME_H

/* Callback pending and resource busy are distinct fields of this state object. */
struct ResourceServiceState {
    unsigned char reserved00[2];
    unsigned char pending;
    unsigned char reserved03[5];
    unsigned initialized;
    unsigned busy;
    unsigned char lock_storage[8];
    unsigned handle;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct ResourceServiceState data_00242afc;
#ifdef __cplusplus
}
#endif

#endif
