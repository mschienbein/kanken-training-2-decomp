/* [INFERENCE] Register/command roles are descriptive. The three direct calls in
 * func_00197774 prepare the payload in the single-precision VFP argument slot;
 * the command wrapper marshals that payload by address, independently of r0. */
extern void func_00187828(unsigned register_address, const void *source,
                          unsigned unit_size, unsigned count,
                          int no_increment, unsigned flags);

void func_001905e0(unsigned register_address, float value)
{
    func_00187828(register_address, &value, 1, 1, 0, 0);
}
