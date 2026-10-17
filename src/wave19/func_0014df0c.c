#include "game/service_runtime.h"

struct DisplayConfiguration {
    unsigned short flags;
    unsigned short mode;
    unsigned short reserved04;
    unsigned short format;
    unsigned buffer;
    unsigned short x;
    unsigned short y;
    unsigned short width;
    unsigned short height;
};
extern int func_00180fe0(struct ServiceCoordinator *coordinator, int index,
                        const struct DisplayConfiguration *configuration);

int func_0014df0c(struct DisplayConfiguration *configurations, int index,
                  const struct DisplayConfiguration *update)
{
    struct DisplayConfiguration *configuration = &configurations[index];
    if ((update->flags & 1) != 0)
        configuration->mode = update->mode;
    if ((update->flags & 4) != 0) {
        configuration->format = update->format;
        configuration->x = update->x;
        configuration->y = update->y;
        configuration->width = update->width;
        configuration->height = update->height;
    }
    if ((update->flags & 2) != 0)
        configuration->buffer = update->buffer;
    return func_00180fe0(&bss_004b5284, index, update);
}
