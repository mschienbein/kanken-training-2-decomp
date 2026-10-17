struct CommandSnapshot {
    unsigned offset;
    unsigned length;
    int first_command;
    int command_count;
};
extern void func_0011ddc0(unsigned *offset, unsigned *length, int *first_command, int *command_count);
extern void func_0019c9d4(unsigned queue);

void func_0019c9b0(struct CommandSnapshot *snapshot, unsigned queue)
{
    func_0011ddc0(&snapshot->offset, &snapshot->length,
                  &snapshot->first_command, &snapshot->command_count);
    func_0019c9d4(queue);
}
