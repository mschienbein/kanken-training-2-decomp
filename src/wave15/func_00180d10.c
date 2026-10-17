struct DriverPayload {
    float master_volume;
    unsigned char reserved04[0x1c];
    short route[2];
    short gain[2];
};
struct DriverCommand {
    unsigned flags;
    struct DriverPayload parameters;
};

struct DriverSelection {
    unsigned char reserved00[0x1322];
    unsigned short command_index;
};
struct DriverCommandTable {
    unsigned char reserved00[0x10a4];
    struct DriverCommand *commands[];
};
/* The indexed table has no recovered bound; the selector is a second typed view. */
union DriverContext {
    struct DriverSelection selection;
    struct DriverCommandTable table;
};

extern void func_00182210(void);
extern int func_0018f018(void);
extern void func_00182204(void);

static __forceinline void set_gain(struct DriverPayload *parameters,
                                   int channel, short value)
{
    parameters->gain[channel] = value;
}

int func_00180d10(union DriverContext *driver, int channel, short value)
{
    struct DriverCommand *command;
    func_00182210();
    if (!func_0018f018()) {
        func_00182204();
        return 0;
    }
    command = driver->table.commands[driver->selection.command_index];
    /* ORIGINAL BUG: the channel is used for the write before the switch. */
    set_gain(&command->parameters, channel, value);
    switch (channel) {
    case 0:
        command->flags |= 0x100;
        break;
    case 1:
        command->flags |= 0x200;
        break;
    }
    func_00182204();
    return 1;
}
