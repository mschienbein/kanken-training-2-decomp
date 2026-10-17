#include "game/matrix.h"
extern "C" void func_00114b5c(struct BasisObject *object, const struct Matrix34 *in)
{
    struct Matrix33 basis;
    func_0012253c(in, &object->translation, &basis);
    object->basis.row0 = basis.row0;
    object->basis.row1 = basis.row1;
    object->basis.row2 = basis.row2;
}
