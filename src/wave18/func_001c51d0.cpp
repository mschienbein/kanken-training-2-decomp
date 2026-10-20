#include "game/slot_bit_owner.h"

/* func_002136d0 registers this as an ObjectDestructor. [INFERENCE] The
 * original class name is unknown; the source destructor has a void contract. */

static __inline void release_slot_bit(unsigned short *flags, unsigned bit)
{
    if ((*flags >> bit) & 1)
        *flags &= ~(1u << bit);
}

SlotBitOwner::~SlotBitOwner()
{
    unsigned bit = index;
    if (bit < 16)
        release_slot_bit(&data_00243124, bit);
}
