#include "game/command_state.h"

/* Full-width incoming identifiers are converted only at the stored-id boundary. */
static void store_position_command(W11PositionCommand *command, short resource, short kind,
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

W11PositionCommand::W11PositionCommand(int resource_value, int kind_value, unsigned option_value,
                                      float x, float y, float z)
{
    store_position_command(this, resource_value, kind_value, option_value, x, y, z);
}
