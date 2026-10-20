#ifndef GAME_SLOT_BIT_OWNER_H
#define GAME_SLOT_BIT_OWNER_H



#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */
struct SlotBitOwner;

struct SlotBitOwner {
    unsigned index;
    ~SlotBitOwner();
};

extern "C" {
extern unsigned short data_00243124;
}
#endif

#endif
