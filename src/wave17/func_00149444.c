#include "game/pool_list.h"

struct PoolElement {
    void *object;
    void *owner;
    unsigned reserved08;
    void *resource;
    unsigned reserved10;
    unsigned char active;
    unsigned char pending;
    unsigned char selected;
    unsigned char enabled;
    unsigned char reserved18[8];
    unsigned short flags;
    unsigned char reserved22[0x36];
    struct PoolLink link;
};

struct PoolElement *func_00149444(struct PoolElement *element)
{
    element->resource = 0;
    element->active = 0;
    element->pending = 0;
    element->selected = 0;
    element->enabled = 0;
    element->flags = 0;
    element->link.next = 0;
    element->link.previous = 0;
    element->object = 0;
    element->owner = 0;
    return element;
}
