#ifndef GAME_RANDOM_H
#define GAME_RANDOM_H

/* [INFERENCE] The low halfword is the signed/unsigned RNG seed; the resetter
 * independently clears the adjacent halfword. Their common storage is evidenced
 * by the same address, not an assumed whole-word seed. */
union RandomSeed {
    unsigned short unsigned_value;
    short signed_value;
};
struct RandomState {
    union RandomSeed seed;
    unsigned short counter;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct RandomState data_00242240;
extern int func_0018ffac(int limit);
#ifdef __cplusplus
}
#endif
#endif
