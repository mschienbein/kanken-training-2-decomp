#include "game/runtime_type_initializers.h"
#include <nw/ut/ut_RuntimeTypeInfo.h>

/* [INFERENCE] Original class identities are unrecovered; the canonical
 * one-parent runtime type layout and constructor describe the observed chain. */
/* INTEGRATOR: differs from neighboring author global RuntimeTypeInfo copies:
 * reuse the existing nw::ut::internal::RuntimeTypeInfo definition and const
 * metadata contract also used by v9a03's data_002434cc caller. */
const nw::ut::internal::RuntimeTypeInfo data_002438ac(&data_002434cc);
