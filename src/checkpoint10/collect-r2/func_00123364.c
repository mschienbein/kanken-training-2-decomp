#include <stddef.h>
#include "ctr/svc.h"

struct AppletCommandValue {
    unsigned value;
    unsigned char enabled;
};

extern CtrResult func_00185e94(unsigned selector, const void *input, unsigned input_size,
                             void *output, unsigned output_size, unsigned *actual_size);

/* [INFERENCE] The original TU/helper boundary is unrecovered. This local
 * no-output wrapper preserves the service's Result contract. */
static CtrResult send_command(unsigned selector, const void *input, unsigned size)
{
    return func_00185e94(selector, input, size, NULL, 0, NULL);
}

void func_00123364(unsigned value, unsigned char enabled)
{
    /* ORIGINAL BUG: the command transmits the structure's uninitialized padding. */
    struct AppletCommandValue input;
    input.value = value;
    input.enabled = enabled;
    send_command(5, &input, sizeof(input));
}
