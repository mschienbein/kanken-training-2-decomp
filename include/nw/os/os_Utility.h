/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/os/os_Utility.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "../../nn/Assert.h"

#include <cstdarg>

#ifdef NW_RELEASE
#define nwosPrintf(...)  ((void)0)
#define nwosVPrintf(...) ((void)0)
#define nwosPanic(...)   ::nw::os::Halt()
#define nwosWarning(...) ((void)0)
#else
#define nwosPrintf       ::nw::os::internal::Printf
#define nwosVPrintf      ::nw::os::internal::VPrintf
#define nwosPanic(...)   ::nw::os::internal::Panic(NN_FILE_NAME, __LINE__, __VA_ARGS__)
#define nwosWarning(...) ::nw::os::internal::Warning(NN_FILE_NAME, __LINE__, __VA_ARGS__)
#endif

namespace nw {
namespace os {
namespace internal {

__weak void Printf(const char* fmt, ...);
__weak void VPrintf(const char* fmt, std::va_list vlist);
__weak void Warning(const char* fileName, int line, const char* fmt, ...);
__weak void Panic(const char* fileName, int line, const char* fmt, ...);
void Halt();

}
}
}

