#include <string.h>
#include "game/service_runtime.h"
static void clear_parameters(struct ChannelParameters *parameters)
{
    memset(parameters, 0, sizeof(*parameters));
}
void func_0017e8fc(struct ChannelWorkspace *workspace)
{
    if (workspace->initialized) {
        unsigned i;
        workspace->initialized = 0;
        clear_parameters(&workspace->parameters);
        for (i = 0; i < workspace->channels; ++i)
            workspace->buffers[i] = 0;
    }
}
