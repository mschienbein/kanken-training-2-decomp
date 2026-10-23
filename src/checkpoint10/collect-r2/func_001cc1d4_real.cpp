#include <preinclude.h>
#include <nw/lyt/lyt_Group.h>

namespace nw{
namespace lyt{

void GroupContainer::AppendGroup(Group* pGroup)
{
    m_GroupList.PushBack(pGroup);
}

}
}
