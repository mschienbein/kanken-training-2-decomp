#include "game/runtime.h"

struct MotionPair {
    float x;
    float y;
    MotionPair();
};

struct LayerSlot {
    unsigned int reserved00;
    unsigned int words[10];
    unsigned char reserved2c[0x4c];
    MotionPair position;
    MotionPair translation;
    MotionPair origin;
    MotionPair scale;
    unsigned char color[4];
    LayerSlot();
};

inline LayerSlot::LayerSlot()
{
    words[0] = 0;
    words[1] = 0;
    words[2] = 0;
    words[3] = 0;
    words[4] = 0;
    words[5] = 0;
    words[6] = 0;
    words[7] = 0;
    words[8] = 0;
    words[9] = 0;
    position.y = position.x = 0.0f;
    scale.y = scale.x = 1.0f;
    color[0] = 255;
    color[1] = 255;
    color[2] = 255;
    color[3] = 255;
}

struct LayerOwner {
    unsigned char reserved00[0x7ec];
    LayerSlot *slots[72];
    void initialize();
};

static LayerSlot *create_slot()
{
    return new (data_002232d5) LayerSlot;
}

void LayerOwner::initialize()
{
    for (int i = 0; i < 72; ++i) {
        if (slots[i] == 0)
            slots[i] = create_slot();
    }
}
