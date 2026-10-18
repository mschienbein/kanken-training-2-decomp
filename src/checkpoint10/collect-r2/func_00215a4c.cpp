#include "nw/ut/ut_RuntimeTypeInfo.h"

/* INTEGRATOR: move to include/game/runtime_type_init.h */
extern "C" {
extern const nw::ut::internal::RuntimeTypeInfo data_002437c4;
}

/* [INFERENCE] The class identity is unrecovered. The relative init-array
 * target initializes this type descriptor's parent link. */
extern const nw::ut::internal::RuntimeTypeInfo data_00243498(&data_002437c4);
