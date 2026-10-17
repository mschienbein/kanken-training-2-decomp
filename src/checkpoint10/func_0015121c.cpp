class CommandTarget;

struct CommandCallbacks {
    void (CommandTarget::*mode)(unsigned char);
    void (CommandTarget::*fade)(signed char, int);
    void (CommandTarget::*value08)(float);
    void (CommandTarget::*value0c)(float);
    void (CommandTarget::*value10)(float);
    void (CommandTarget::*value14)(float);
    void (CommandTarget::*value104)(float);
    void (CommandTarget::*value108)(float, int);
    void (CommandTarget::*value18)(float);
    void (CommandTarget::*flag83)(unsigned char);
    void (CommandTarget::*flagf4)(signed char);
    void (CommandTarget::*flagf0)(unsigned char);
};

/* INTEGRATOR: move to include/game/command_group.h */
extern "C" {
    extern const CommandCallbacks data_0021df3c;
}

struct CommandGate {
    unsigned values[2];
    unsigned char enabled;
    unsigned char reserved[3];
};

struct CommandGroup {
    unsigned char reserved0[0x90];
    CommandTarget *children[16];
    unsigned char reserved1[0x134 - 0xd0];
    CommandGate gates[4];

    CommandTarget *child(int index) const {
        /* ORIGINAL BUG: negative indices are not rejected. */
        if (index > 15)
            return 0;
        return children[index];
    }
};


extern "C" void func_0015121c(CommandGroup *group, unsigned mask, unsigned char value)
{
    int i = 0;
    void (CommandTarget::*callback)(unsigned char) = data_0021df3c.mode;
    for (; i < 16; ++i) {
        if (mask == 0)
            break;
        if (mask & 1) {
            CommandTarget *target = group->child(i);
            if (target == 0) {
                mask >>= 1;
                continue;
            }
            (target->*callback)(value);
        }
        mask >>= 1;
    }
}
