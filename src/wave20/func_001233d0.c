#include "game/framebuffer.h"
#include "game/reset_reference.h"
#include "game/service_protocol.h"

/* [INFERENCE] Helper factoring does not establish the historical TU boundary. */
static long long display_timeout(void)
{
    return data_00242a40.nanoseconds;
}

static int send_display_layout(unsigned channel, unsigned command,
                               const struct DisplayMemoryLayout *layout)
{
    return func_00185fc0(channel, command, layout, sizeof(*layout),
                        data_00242a34, display_timeout());
}

int func_001233d0(unsigned channel)
{
    struct DisplayFramebufferPair pair;
    struct DisplayFramebuffer *current = pair.display;
    /* The exact generic runtime callback returns its storage unchanged.
     * The original constructor leaves every element field uninitialized. */
    __aeabi_vec_ctor_nocookie_nodtor(current, func_00142a54,
                                    sizeof(*current), 2);
    func_00142b8c(&pair);
    func_00142bf0(channel, display_timeout());
    bss_0049ee8c.lower_format = current[0].format;
    bss_0049ee8c.upper_format = current[1].format;
    bss_0049ee8c.stereo = current[0].left != current[0].right;
    func_00142e70(&bss_0049ee8c);
    send_display_layout(channel, 0x10, &bss_0049ee8c);
    func_00142d74(channel, 0);
    func_00142fa4(&bss_0049ee8c, sizeof(bss_0049ee8c));
    return 0;
}
