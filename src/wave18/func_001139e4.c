#include "game/input.h"

void func_001139e4(struct InputCalibration *calibration, int minimum, int maximum)
{
    if (maximum > 145)
        maximum = 145;
    switch (calibration->mode) {
    case 0:
        if (minimum < 40)
            minimum = 40;
        calibration->minimum[0] = minimum;
        calibration->maximum[0] = maximum;
        break;
    case 1:
        if (minimum < 36)
            minimum = 36;
        calibration->minimum[1] = minimum;
        calibration->maximum[1] = maximum;
        break;
    default:
        calibration->maximum[2] = maximum;
        break;
    }
}
