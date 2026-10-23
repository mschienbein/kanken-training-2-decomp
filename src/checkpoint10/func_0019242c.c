#include <stddef.h>
#include "ctr/svc.h"

extern CtrResult func_00185e94(unsigned selector, const void *input, unsigned input_size,
                             void *output, unsigned output_size, unsigned *actual_size);

static __inline void send_command(unsigned selector, const void *input, unsigned input_size)
{
    func_00185e94(selector, input, input_size, NULL, 0, NULL);
}

void func_0019242c(void)
{
    send_command(4, NULL, 0);
}
