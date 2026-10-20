#ifndef GAME_LAYOUT_RESOURCES_H
#define GAME_LAYOUT_RESOURCES_H
#include <preinclude.h>
#include <nw/lyt/lyt_Group.h>
#include <nw/lyt/lyt_Layout.h>
/* [INFERENCE] Explicit group-ownership phase, not an alias of the complete
 * GroupContainer destructor (whose member clear has a different extent). */
extern "C" nw::lyt::GroupContainer *func_001cc1e4(nw::lyt::GroupContainer *container);
#endif
