#ifndef GAME_INPUT_H
#define GAME_INPUT_H

struct InputController;

struct InputCalibration {
    short minimum[3], maximum[3];
    unsigned char mode, reserved0d;
    short threshold;
    float scale, limit, offset[3];
};

struct ButtonRing;
struct InputService {
    unsigned handle;
    const volatile struct ButtonRing *ring;
};

struct InputReader {
    struct InputService *service;
    int index;
    unsigned previous_held;
    struct InputCalibration calibration;
    unsigned char first_sample;
    unsigned char reserved31[7];
    long long previous_time;
};

struct ButtonSample {
    unsigned held;
    unsigned pressed;
    unsigned released;
    short x, y;
};

struct InputServiceConfiguration {
    const void *reserved00[2];
    const char *service_name;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct InputService bss_004b1fb0;
extern const struct InputServiceConfiguration data_0021b0ec;
extern unsigned char data_00242c50[];
extern struct InputController *func_00185044(void);
extern void func_00137128(struct InputController *controller);
extern int func_001137c4(struct InputService *service, const char *service_name);
extern void func_001139e4(struct InputCalibration *calibration, int minimum, int maximum);
extern float func_00113a34(const struct InputCalibration *calibration, int coordinate);
extern void func_00113ab0(struct InputCalibration *calibration);
extern void func_00113afc(const struct InputCalibration *calibration, short *x, short *y,
                         int raw_x, int raw_y);
extern int func_00113bf4(void);
extern void func_00115c40(const volatile struct ButtonRing *ring, struct ButtonSample *samples,
                         int capacity, int *count, long long *previous_time, int *previous_index);
extern int func_00121354(void);
extern void func_00123a64(struct ButtonSample *sample);
extern void func_00123a70(unsigned *held, unsigned *pressed, unsigned *released);
#ifdef __cplusplus
}
#endif

#endif
