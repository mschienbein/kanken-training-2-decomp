#include "game/audio_tasks.h"
#include "game/service_result.h"

namespace {
struct TaskIterator {
    PoolLink *node;
    explicit TaskIterator(PoolLink *value) : node(value) {}
};
static TaskIterator append_link(PoolList &list, PoolLink &node)
{
    return TaskIterator(func_00191154(&list, &list.sentinel, &node));
}
}

/* [INFERENCE] This implementation-private view names the existing request
 * activation and intrusive-link operations; no historical class name is claimed. */
namespace {
struct WorkRequestView {
    AudioWorkRequest *request;
    explicit WorkRequestView(AudioWorkRequest *value) : request(value) {}
    void activate() const {
        func_0018221c(&request->signal);
        request->active = 1;
    }
    PoolLink &link() const { return request->link; }
};
}

void func_0017df84(AudioTaskManager *manager, AudioWorkRequest *request,
                   unsigned category)
{
    WorkRequestView work(request);
    const unsigned selected = category;

    StreamMutex *guard = &manager->lock;
    func_00192bd8(guard);
    work.activate();
    append_link(manager->lists[selected], work.link());
    func_0014a7e0(&manager->queue, 0);
    func_00195d74(guard);
}

