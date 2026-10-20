#ifndef GAME_CONTROL_TRIPLET_H
#define GAME_CONTROL_TRIPLET_H



#ifdef __cplusplus
/* [INFERENCE] Native identities and roles; layouts follow matching consumers. */
struct ControlTriplet;

struct ControlTriplet {
    unsigned char enabled0;
    unsigned char active0;
    float x0;
    float y0;
    unsigned char enabled1;
    unsigned char active1;
    float x1;
    float y1;
    unsigned char enabled2;
    float x2;
    float y2;
    ControlTriplet();
    void clear();
};

extern "C" {

}
#endif

#endif
