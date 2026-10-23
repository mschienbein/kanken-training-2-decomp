/* [INFERENCE] Element identity and value meaning are unrecovered. The
 * initializer at 0x001f5980 constructs twenty eight-byte elements at
 * bss_004b92b8 with this callback and __aeabi_vec_ctor_nocookie_nodtor.
 * The constructor callback preserves its object pointer as its ABI result.
 * The single 64-bit value is inferred from the combined zero assignment. */
struct ConstructorPair00156dbc {
    unsigned long long value;
};

struct ConstructorPair00156dbc *func_00156dbc(struct ConstructorPair00156dbc *object)
{
    object->value = 0;
    return object;
}
