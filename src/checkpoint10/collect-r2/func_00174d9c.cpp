/* INTEGRATOR: differs from include/game/command_state.h: the constructor takes the resource and kind
 * ids as `short` (halfword parameters reproduce the original r4/r5/r6 save order and the vtable-first
 * store order); the header declares them as int. Header class layout is otherwise identical. */
struct W11BaseCommand {
    W11BaseCommand();
    virtual ~W11BaseCommand();
};

struct W11PositionCommand : W11BaseCommand {
    W11PositionCommand();
    W11PositionCommand(short resource, short kind, unsigned option, float x, float y, float z);
    virtual ~W11PositionCommand();
    short resource;
    short kind;
    unsigned short flags;
    unsigned short reserved0a;
    float position[3];
    unsigned option;
};

static void init_position_command(W11PositionCommand *command, short resource, short kind,
                                  unsigned option, float x, float y, float z)
{
    command->resource = resource;
    command->kind = kind;
    command->flags = 0;
    command->position[0] = x;
    command->position[1] = y;
    command->position[2] = z;
    command->option = option;
}

W11PositionCommand::W11PositionCommand(short resource_value, short kind_value, unsigned option_value,
                                       float x, float y, float z)
{
    init_position_command(this, resource_value, kind_value, option_value, x, y, z);
}
