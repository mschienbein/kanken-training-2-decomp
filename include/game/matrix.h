#ifndef GAME_MATRIX_H
#define GAME_MATRIX_H

struct Matrix44 {
    float elements[16];
};

struct Matrix34 {
    float elements[12];
};

struct MatrixObject {
    unsigned reserved00;
    struct Matrix44 matrix;
    struct Matrix34 view;
};

#ifdef __cplusplus
extern "C" {
#endif
/* [INFERENCE] Both helpers preserve the destination in r0. Observed callers
 * ignore the result; pointer returns retain the established wave-5 contract. */
extern struct Matrix44 *func_001944f0(struct Matrix44 *destination, const struct Matrix44 *source);
extern struct Matrix34 *func_00195334(struct Matrix34 *destination, const struct Matrix34 *source);
#ifdef __cplusplus
}
#endif

#endif
