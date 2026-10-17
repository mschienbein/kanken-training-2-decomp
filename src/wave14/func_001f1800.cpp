#include "game/event_route.h"
/* [INFERENCE] Vtable-only notification entry; no caller establishes a return value. */
void EventDispatcher::dispatch(RoutedEvent *event)
{
    if ((!priority_receiver || !priority_receiver->receive(event)) &&
        !func_001f261c(this, event))
        receive(event);
}
