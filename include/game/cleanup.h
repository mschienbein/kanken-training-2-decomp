#ifndef GAME_CLEANUP_H
#define GAME_CLEANUP_H

struct ConstructedVtable;
struct BaseObject {
    const struct ConstructedVtable *vtable;
};
#ifdef __cplusplus
extern "C" {
#endif
extern const struct ConstructedVtable data_00224890;
#ifdef __cplusplus
}
#endif

#endif
