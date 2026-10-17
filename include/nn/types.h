/* SPDX-License-Identifier: GPL-3.0-or-later
 * Project-owned declarations inferred only from the public NW4C source's use.
 * No Nintendo SDK header was consulted or copied. */
#pragma once
#include "../nw/types.h"
namespace nn {
class Result {
    u32 m_Value;
public:
    bool IsSuccess() const;
    bool IsFailure() const;
};
}
