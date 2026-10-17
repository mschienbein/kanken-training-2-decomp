struct AudioChannelState;
/* INTEGRATOR: differs from include/game/audio_voice.h: the reserved00 region
 * contains a signed queue priority at offset 0x20 (written here, 0x20 bytes
 * into the channel, with the clamped value also forwarded to func_00156484). */
struct AudioChannel {
    unsigned char reserved00[0x20];
    int priority;
    unsigned char reserved24[0x44];
    struct AudioChannelState *state;
};
struct AudioChannelQueue;
extern struct AudioChannelQueue bss_004b6888;
extern void func_00156484(struct AudioChannelQueue *queue,
                          struct AudioChannel *channel, int priority);

/* Same shapes as nw::ut::Min/Max (include/nw/ut/ut_Inlines.h). */
static __inline int Max(int a, int b) { return a < b ? b : a; }
static __inline int Min(int a, int b) { return b < a ? b : a; }

void func_00155dec(struct AudioChannel *channel, int priority)
{
    priority = Min(priority, 0x7fff);
    priority = Max(priority, 0);
    channel->priority = priority;
    func_00156484(&bss_004b6888, channel, priority);
}
