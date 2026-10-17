#include "game/input.h"
#include "game/service_state.h"

int func_0010c1d4(struct InputReader *reader, struct ButtonSample *sample)
{
    long long previous_time = -1;
    int previous_index = -1;
    int count;
    if (func_00113bf4() == 0) {
        struct InputService *service;
        func_00113ab0(&reader->calibration);
        service = reader->service;
        func_00115c40(service->ring, sample, 1, &count, &previous_time, &previous_index);
        if (count > 0) {
            func_00113afc(&reader->calibration, &sample->x, &sample->y, sample->x, sample->y);
            if (reader->first_sample) {
                reader->previous_held = sample->held;
                reader->first_sample = 0;
            }
            sample->held &= ~0x2000u;
            sample->pressed = (sample->held ^ reader->previous_held) & ~reader->previous_held;
            sample->released = reader->previous_held & ~sample->held;
            if (func_001927a8() != 0 && func_00192c10() == 0) {
                sample->held = 0;
                sample->released = 0;
                sample->pressed = 0;
                sample->x = 0;
                sample->y = 0;
            }
            reader->previous_held = sample->held;
            if (data_00242c50[0] == 0)
                func_00123a64(sample);
            return 1;
        }
    }
    return 0;
}
