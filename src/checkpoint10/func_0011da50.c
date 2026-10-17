extern void func_00187828(unsigned register_address, const void *source,
                          unsigned unit_size, unsigned count,
                          int no_increment, unsigned flags);

void func_0011da50(unsigned register_address, unsigned count,
                   int increment, const void *source)
{
    func_00187828(register_address, source, 4, count, !increment, 1);
}
