#include "game/input.h"

static __forceinline void setAxes(short *axes, short x, short y, short z)
{
    axes[0] = x;
    axes[1] = y;
    axes[2] = z;
}

static __forceinline void setCalibrationFloats(struct InputCalibration *calibration,
                                              float offset, float scale, float limit)
{
    calibration->offset[0] = offset;
    calibration->offset[1] = offset;
    calibration->offset[2] = offset;
    calibration->scale = scale;
    calibration->limit = limit;
}

struct InputCalibration *func_00113b90(struct InputCalibration *calibration)
{
    short maximum = 145;
    calibration->minimum[0] = 40;
    calibration->minimum[1] = 36;
    calibration->minimum[2] = 40;
    setAxes(calibration->maximum, maximum, maximum, maximum);
    calibration->mode = 0;
    calibration->threshold = 141;
    setCalibrationFloats(calibration, 0.0f, 1.5f, 141.0f);
    return calibration;
}
