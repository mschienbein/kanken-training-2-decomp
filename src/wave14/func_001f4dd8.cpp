#include "game/event_route.h"
/* [INFERENCE] Inlined delivery helper; the original helper identity is unrecovered. */
inline void EventDispatcher::deliver(RoutedEvent *event)
{
    if ((!priority_receiver || !priority_receiver->receive(event)) &&
        !func_001f261c(this, event))
        receive(event);
}

/* [INFERENCE] Vtable-only notification entry; no caller establishes a return value. */
void EventRoute::dispatch(RoutedEvent *event)
{
    static_cast<EventDispatcher *>(this)->deliver(event);
}
