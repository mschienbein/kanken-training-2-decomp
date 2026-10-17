#include "game/functions.h"
extern void func_00182dc8(unsigned target, int level, unsigned internal_format,
    int width, int height, int border, unsigned format, unsigned type, const void *pixels);

extern void func_00182d9c(unsigned target, unsigned parameter, float value);

void func_001db434(int width, int height, unsigned kind, const void *pixels,
    int linear_minimum, int linear_maximum)
{
    unsigned format = 0x6756;
    unsigned type = 0x1401;
    switch (kind) {
    case 11: type = 0x6761; break;
    case 9: format = 0x6758; type = 0x6760; break;
    case 5: format = 0x6758; break;
    case 4: format = 0x6752; type = 0x8033; break;
    case 2: format = 0x6752; type = 0x8034; break;
    case 0: format = 0x6752; break;
    case 3: format = 0x6754; type = 0x8363; break;
    case 1: format = 0x6754; break;
    case 6: case 7: case 8: case 10: break;
    }
    func_00182dc8(0x02010de1, 0, format, width, height, 0, format, type, pixels);
    func_00182db4(0x0de1, 0x2802, 0x812f);
    func_00182db4(0x0de1, 0x2803, 0x812f);
    func_00182db4(0x0de1, 0x2801, linear_minimum ? 0x2601 : 0x2600);
    func_00182db4(0x0de1, 0x2800, linear_maximum ? 0x2601 : 0x2600);
    func_00182d9c(0x0de1, 0x8501, 0.0f);
    func_00182db4(0x0de1, 0x813a, -1000);
}
