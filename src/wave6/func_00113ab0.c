#include "game/input.h"

void func_00113ab0(struct InputCalibration *calibration)
{
    if (calibration->minimum[0] < 40)
        calibration->minimum[0] = 40;
    if (calibration->minimum[1] < 36)
        calibration->minimum[1] = 36;
    if (calibration->maximum[0] > 145)
        calibration->maximum[0] = 145;
    if (calibration->maximum[1] > 145)
        calibration->maximum[1] = 145;
    if (calibration->maximum[2] > 145)
        calibration->maximum[2] = 145;
}
