/* [INFERENCE] The helper consumes count command words from a read-only source. */
extern void func_00187828(unsigned register_address, const void *source,
                          unsigned unit_size, unsigned count,
                          int no_increment, unsigned flags);

void func_001905b0(unsigned register_address, unsigned count, const void *source)
{
    func_00187828(register_address, source, 4, count, 0, 0);
}
