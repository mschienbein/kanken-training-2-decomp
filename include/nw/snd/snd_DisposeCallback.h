/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_DisposeCallback.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: retain public class layouts; normalize standalone release include paths.
 * This file is free software under GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_DISPOSE_CALLBACK_H_
#define NW_SND_DISPOSE_CALLBACK_H_

#include <nw/ut/ut_LinkList.h>

namespace nw { 
namespace snd { 
namespace internal { 
namespace driver {

class DisposeCallback
{
public:
    ut::LinkListNode m_DisposeLink;

    virtual ~DisposeCallback();
    virtual void InvalidateData(const void* start, const void* end) = 0;
};

} // namespace nw
} // namespace snd
} // namespace internal
} // namespace driver

#endif // NW_SND_DISPOSE_CALLBACK_H_

