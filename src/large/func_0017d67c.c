#include "game/hwr_runtime.h"
int func_0017d67c(uint16_t first, uint16_t count, uint16_t *min_x,
                  uint16_t *min_y, uint16_t *max_x, uint16_t *max_y)
{
    uint16_t i;
    if (bss_004e1e04[data_00243888[first] + count].max_y != 0) {
        *min_x = bss_004e1e04[data_00243888[first] + count].min_x;
        *min_y = bss_004e1e04[data_00243888[first] + count].min_y;
        *max_x = bss_004e1e04[data_00243888[first] + count].max_x;
        *max_y = bss_004e1e04[data_00243888[first] + count].max_y;
    } else {
        *min_x = data_0024385c->geometry.min_x[first];
        *min_y = data_0024385c->geometry.min_y[first];
        *max_x = data_0024385c->geometry.max_x[first];
        *max_y = data_0024385c->geometry.max_y[first];
        for (i = first + 1; i < first + count; ++i) {
            *min_x = data_0024385c->geometry.min_x[i] <= *min_x
                ? data_0024385c->geometry.min_x[i] : *min_x;
            *min_y = data_0024385c->geometry.min_y[i] <= *min_y
                ? data_0024385c->geometry.min_y[i] : *min_y;
            *max_x = data_0024385c->geometry.max_x[i] > *max_x
                ? data_0024385c->geometry.max_x[i] : *max_x;
            *max_y = data_0024385c->geometry.max_y[i] > *max_y
                ? data_0024385c->geometry.max_y[i] : *max_y;
        }
        bss_004e1e04[data_00243888[first] + count].min_x = *min_x;
        bss_004e1e04[data_00243888[first] + count].min_y = *min_y;
        bss_004e1e04[data_00243888[first] + count].max_x = *max_x;
        bss_004e1e04[data_00243888[first] + count].max_y = *max_y;
    }
    return 1;
}
