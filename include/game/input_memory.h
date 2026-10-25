#ifndef GAME_INPUT_MEMORY_H
#define GAME_INPUT_MEMORY_H

/* Shared input-service mapping, not a runtime-object overlay. The initializer
 * publishes these five bank addresses from one0x2b0-byte kernel mapping.
 * Bank contents remain opaque; only their evidenced spans are completed here. */
struct ButtonRing { unsigned char reserved00[0xa8]; };
struct TouchRing { unsigned char reserved00[0x60]; };
struct SensorRing { unsigned char reserved00[0x50]; };
struct MotionRing { unsigned char reserved00[0xe0]; };
struct AuxiliaryInputRing { unsigned char reserved00[0x78]; };
struct InputSharedMemory {
    struct ButtonRing buttons;
    struct TouchRing touch;
    struct SensorRing sensor;
    struct MotionRing motion;
    struct AuxiliaryInputRing auxiliary;
};
#endif
