/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/ut/ut_MiddlewareString.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_UT_MIDDLEWARESTRING_H
#define NW_UT_MIDDLEWARESTRING_H

#include "../Assert.h"
#include "../version.h"
#include "../../nn/module.h"

#define NW_MACROTO_STR_IMPL(x)          #x
#define NW_MACROTO_STR(macroArg)  NW_MACROTO_STR_IMPL(macroArg)

#define NW_MODULE_SYMBOL(module_name) "NW4C_" NW_MACROTO_STR(NW_VERSION_MAJOR) "_" NW_MACROTO_STR(NW_VERSION_MINOR) "_" NW_MACROTO_STR(NW_VERSION_MICRO) "_" #module_name
#define NW_PUT_MODULE_SYMBOL(module_name)                                            \
NN_MAKE_MODULE(moduleInfo, "NINTENDO", NW_MIDDLEWARE_SYMBOL(module_name));         \
NN_REFER_MODULE(moduleInfo);                                                         \

#endif // NINTENDOWARE_UT_MIDDLEWARESTRING_H

