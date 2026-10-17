struct ResourceFile;

/* [INFERENCE] Member slot +4 of the resource vtable installed by
 * func_001c47b8. Preserve its object input even though this error path ignores
 * it. The unused operation-specific inputs are unresolved, not proven absent. */
unsigned func_001c47a8(struct ResourceFile *file)
{
    return 0xe0c046f8u;
}
