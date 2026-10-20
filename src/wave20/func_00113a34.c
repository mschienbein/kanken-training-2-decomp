#include "game/input.h"

float func_00113a34(const struct InputCalibration *calibration, int coordinate)
{
    int range;
    float value = (float)coordinate;
    /* ORIGINAL BUG: modes other than 0, 1 and 2 leave the range uninitialized. */
    switch (calibration->mode) {
    case 0:
        range = (short)(calibration->maximum[0] - calibration->minimum[0]);
        break;
    case 1:
        range = (short)(calibration->maximum[1] - calibration->minimum[1]);
        break;
    case 2:
        range = 105;
        break;
    }
    if (coordinate == 0)
        return 0.0f;
    if (range <= coordinate)
        return 1.0f;
    return value / (float)range;
}
