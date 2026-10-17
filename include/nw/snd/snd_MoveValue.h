/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_MoveValue.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_MOVE_VALUE_H_
#define NW_SND_MOVE_VALUE_H_

namespace nw { 
namespace snd { 
namespace internal {

template <typename ValueType, typename CountType>
class MoveValue
{
public:
    MoveValue(): 
        m_Origin(0), 
        m_Target(0), 
        m_Frame(0), 
        m_Counter(0)
        {
    }

    void InitValue(ValueType value)
    {
        m_Origin  = value;
        m_Target  = value;
        m_Frame   = 0;
        m_Counter = 0;
    }

    void SetTarget(ValueType targetValue, CountType frames)
    {
        m_Origin  = GetValue();
        m_Target  = targetValue;
        m_Frame   = frames;
        m_Counter = 0;
    }

    ValueType GetTarget() const
    {
        return m_Target;
    }

    ValueType GetValue() const
    {
        if (IsFinished())
            return m_Target;

        return static_cast<ValueType>(m_Origin + (m_Target - m_Origin) * ValueType(m_Counter) / ValueType(m_Frame));
    }

    void Update()
    {
        if (m_Counter < m_Frame)
            ++m_Counter;
    }

    bool IsFinished() const
    {
        return m_Counter >= m_Frame;
    }

    CountType GetRemainingCount() const
    {
        if (IsFinished())
            return static_cast<CountType>(0);

        return m_Frame - m_Counter;
    }

private:
    ValueType m_Origin;
    ValueType m_Target;
    CountType m_Frame;
    CountType m_Counter;
};

} // namespace internal
} // namespace snd
} // namespace nw

#endif // NW_SND_MOVE_VALUE_H_

