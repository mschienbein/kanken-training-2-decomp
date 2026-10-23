#include "game/input.h"

extern short func_001236b0(short *x, short *y, int raw_x, int raw_y, int minimum, int maximum);
extern short func_001237b4(short *x, short *y, int raw_x, int raw_y, int minimum, int maximum);
extern void func_00123898(short *x, short *y, int raw_x, int raw_y, int minimum, int maximum);

void func_00113afc(const struct InputCalibration *calibration, short *x, short *y,
                   int raw_x, int raw_y)
{
    switch (calibration->mode) {
    case 0:
        func_001237b4(x, y, raw_x, raw_y, calibration->minimum[0], calibration->maximum[0]);
        break;
    case 1:
        func_001236b0(x, y, raw_x, raw_y, calibration->minimum[1], calibration->maximum[1]);
        break;
    case 2:
        func_00123898(x, y, raw_x, raw_y, calibration->minimum[2], calibration->maximum[2]);
        break;
    }
}
