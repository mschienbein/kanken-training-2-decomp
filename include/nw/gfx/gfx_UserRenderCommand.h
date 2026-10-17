/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/gfx_UserRenderCommand.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "gfx_RenderElement.h"

#include <functional>

namespace nw{
namespace gfx{

class UserRenderNode;

class UserRenderCommand : public RenderCommand
{
private:
    NW_DISALLOW_COPY_AND_ASSIGN(UserRenderCommand);
    
public:
    virtual void Invoke(RenderContext* renderContext) = 0;

    void SetUserRenderNode(UserRenderNode* userRenderNode)
    {
        m_Parent = userRenderNode;
    }

    UserRenderNode* GetUserRenderNode() const
    {
        return m_Parent;
    }

protected:
    UserRenderCommand() {}
    virtual ~UserRenderCommand() {}

    UserRenderNode* m_Parent;
};

}
}

