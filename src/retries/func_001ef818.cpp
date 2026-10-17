#include "game/kernel_owners.h"
#include "ctr/svc.h"

/* [INFERENCE] The erased return-only callee at func_001ef884 is bound as the
 * EventPart28 destructor; its callee identity is not recoverable from the no-op. */

LargeEventOwner::~LargeEventOwner()
{
}
