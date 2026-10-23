#include <float.h>

class EndianWord {
public:
    unsigned bits;
    EndianWord();
    EndianWord &operator=(unsigned value);
};
class StateObject {
public:
    EndianWord first;
    EndianWord second;
    EndianWord values[4];
    EndianWord options[2];
    unsigned char active;
    unsigned char reserved21[3];
    float scale_x, scale_y, x, y, z, weight;
    unsigned mode, flags, count;
    unsigned char enabled, selection;
    StateObject();
};
class ExtendedState : public StateObject {
public:
    float maximum, elapsed, duration;
    unsigned status, reserved5c;
    const void *table;
    ExtendedState();
};

/* INTEGRATOR: move to include/game/state_object.h */
extern "C" {
    extern const unsigned data_002438cc[];
}

ExtendedState::ExtendedState()
{
    table = data_002438cc;
    maximum = FLT_MAX;
    status = 4;
    reserved5c = 0;
    elapsed = 0.0f;
    duration = 0.0f;
}
