#include "game/audio_tasks.h"

void func_0017df50(struct AudioWorkItem *item)
{
    item->field54 = 0;
    item->request.owner = item;
    item->request.context = item;
    item->field00 = item->field14;
    func_0017df84(func_00185de4(), &item->request, 1);
}
