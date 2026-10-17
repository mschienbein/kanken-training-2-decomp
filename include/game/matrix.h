#ifndef GAME_MATRIX_H
#define GAME_MATRIX_H

struct Matrix44 {
    float elements[16];
};

struct Matrix34 {
    float elements[12];
#ifdef __cplusplus
    Matrix34() {} /* Matrix temporaries intentionally retain uninitialized storage. */
    Matrix34(const Matrix34 &source);
#endif
};

struct MatrixObject {
    unsigned reserved00;
    struct Matrix44 matrix;
    struct Matrix34 view;
};

struct Vector3 { float x, y, z; };
struct Matrix33 { struct Vector3 row0, row1, row2; };
struct BasisObject { unsigned char reserved00[24]; struct Vector3 translation; struct Matrix33 basis; };
#ifdef __cplusplus
extern "C" {
#endif
/* [INFERENCE] Both helpers preserve the destination in r0. Observed callers
 * ignore the result; pointer returns retain the established wave-5 contract. */
extern void func_0012253c(const struct Matrix34 *in, struct Vector3 *translation, struct Matrix33 *basis);
extern struct Matrix44 *func_001944f0(struct Matrix44 *destination, const struct Matrix44 *source);
extern struct Matrix34 *func_00195334(struct Matrix34 *destination, const struct Matrix34 *source);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
inline Matrix34::Matrix34(const Matrix34 &source)
{
    func_00195334(this, &source);
}
#endif

#endif
