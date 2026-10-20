#include <stddef.h>
#include "game/screen.h"
#include "game/runtime.h"

struct LayerPair { float x, y; };
struct PanelLayer {
    unsigned short category;
    unsigned short state;
    struct DisplayObject *objects[10];
    unsigned char reserved2c[0x4c];
    struct LayerPair position;
    struct LayerPair rotation;
    struct LayerPair reserved88;
    struct LayerPair scale;
    unsigned char color[4];
};

struct LayerManager {
    int bits_per_word;
    int word_count;
    unsigned int masks[1];
    unsigned char reserved0c[0x7e0];
    struct PanelLayer *layers[72];
};

void func_001ea9f8(struct LayerManager *manager, int index)
{
    int i;
    struct PanelLayer *layer = manager->layers[index];
    if (layer) {
        layer->category = 0;
        layer->state = 0;
        layer->position.x = layer->position.y = 0.0f;
        layer->rotation.x = layer->rotation.y = 0.0f;
        layer->scale.x = layer->scale.y = 1.0f;
        layer->color[0] = layer->color[1] = layer->color[2] = layer->color[3] = 255;
        for (i = 0; i < 10; ++i)
            layer->objects[i] = 0;
    }
}
