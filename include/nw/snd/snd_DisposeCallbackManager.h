/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_DisposeCallbackManager.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: retain public class layouts; normalize standalone release include paths.
 * This file is free software under GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_DISPOSE_CALLBACK_MANAGER_H_
#define NW_SND_DISPOSE_CALLBACK_MANAGER_H_

#include <nw/ut/ut_LinkList.h>
#include <nw/snd/snd_DisposeCallback.h>

namespace nw {
namespace snd {
namespace internal {
namespace driver {

class DisposeCallbackManager
{
public:
    typedef ut::LinkList< DisposeCallback, offsetof(DisposeCallback,m_DisposeLink) > CallbackList;

public:
    static DisposeCallbackManager& GetInstance();

    void Dispose(void* mem, unsigned long size);

    void RegisterDisposeCallback(DisposeCallback* callback);
    void UnregisterDisposeCallback(DisposeCallback* callback);

    unsigned long GetCallbackCount() const;

private:
    DisposeCallbackManager();

    CallbackList m_CallbackList;
};

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw

#endif // NW_SND_DISPOSE_CALLBACK_MANAGER_H_

