#ifndef GAME_HEAP_INTERFACE_H
#define GAME_HEAP_INTERFACE_H

#ifdef __cplusplus
/* [INFERENCE] Native heap identity; allocation slot follows matching consumers. */
class Heap {
public:
    virtual ~Heap();
    virtual void *allocate(unsigned int size, unsigned int alignment);
};
#endif

#endif
