#include "game/input.h"
#include <stddef.h>
struct InputReader *func_0010c304(struct InputReader *reader, struct InputService *service)
{
    struct InputCalibration *calibration;
    reader->service = service;
    reader->index = -1;
    calibration = func_00113b90(&reader->calibration);
    /* Recover the containing reader from the initializer's returned member. */
    reader = (struct InputReader *)((char *)calibration - offsetof(struct InputReader, calibration));
    reader->first_sample = 1;
    reader->previous_time = -1;
    return reader;
}
