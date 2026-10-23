#ifndef GAME_INPUT_H
#define GAME_INPUT_H

#include <stdbool.h>
#include "ctr/svc.h"
#include "game/memory_arena.h"

struct InputController;

struct InputCalibration {
    short minimum[3], maximum[3];
    unsigned char mode, reserved0d;
    short threshold;
    float scale, limit, offset[3];
};

struct ButtonRing;
struct TouchRing;
struct SensorRing;
struct MotionRing;
struct AuxiliaryInputRing;
struct MotionInputVTable;
struct TouchInputService {
    CtrHandle handle;
    const volatile struct TouchRing *ring;
};
struct SensorInputService {
    CtrHandle handle;
    const volatile struct SensorRing *ring;
};
struct MotionInputService {
    const struct MotionInputVTable *vtable;
    CtrHandle handle;
    const volatile struct MotionRing *ring;
};
struct AuxiliaryInputService {
    CtrHandle handle;
    const volatile struct AuxiliaryInputRing *ring;
};
/* Initializer001f58d4 and cleanup001c5690 establish this complete aggregate;
 * getter0010c12c returns its actual touch subobject at +8. */
struct InputService {
    CtrHandle handle;
    const volatile struct ButtonRing *ring;
    struct TouchInputService touch;
    struct SensorInputService sensor;
    struct MotionInputService motion;
    struct AuxiliaryInputService auxiliary;
    struct MappedMemoryBlock mapping;
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

struct InputRectangle {
    float x, y, width, height;
};

struct InputPoint {
    float x, y;
};

struct InputSnapshot {
    bool active, released;
    unsigned char reserved02[2];
    struct InputPoint position;
    bool previous_active, previous_released;
    unsigned char reserved0e[2];
    struct InputPoint previous_position;
    bool tracking;
    unsigned char reserved19[3];
    struct InputPoint start_position;
    struct InputRectangle rectangle;
    unsigned int state;
};

struct InputMode {
    unsigned int value;
};

struct InputItem {
    unsigned char reserved00[0x0c];
    unsigned short flags;
    unsigned char reserved0e;
    unsigned char state;
    unsigned char reserved10[2];
    unsigned short counter;
};

struct InputGroup {
    unsigned int flags;
    int count;
    struct InputItem **items;
    struct InputSnapshot input;
    unsigned char reserved44[0x3c];
    unsigned int enabled;
    struct InputMode mode;
};

struct InputOwner {
    unsigned char reserved00[0x20];
#ifndef __cplusplus
    struct InputGroup *groups[];
#endif
};

struct TouchDeviceState {
    unsigned char reserved00[4];
    unsigned direction;
    unsigned char reserved08[4];
    unsigned int active;
    unsigned buttons;
    unsigned reserved14;
    unsigned flags;
    unsigned char reserved1c[8];
    unsigned int released;
    unsigned char reserved28[0x10];
    struct InputPoint position;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct TouchDeviceState bss_004972d8;
extern struct InputService bss_004b1fb0;
extern const struct InputServiceConfiguration data_0021b0ec;
extern unsigned char data_00242c50[];
extern struct InputController *func_00185044(void);
extern void func_00137128(struct InputController *controller);
extern int func_001137c4(struct InputService *service, const char *service_name);
extern struct InputCalibration *func_00113b90(struct InputCalibration *calibration);
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
extern struct TouchDeviceState bss_004972d8;
extern void func_00170418(struct InputSnapshot *input);
extern int func_0016ec14(const struct InputMode *mode);
extern int func_001703c4(const struct InputRectangle *rectangle, float x, float y);
#ifdef __cplusplus
}
#endif

#endif
