struct ResourceFile;

/* [INFERENCE] Member slot +0x10 of the resource vtable installed by
 * func_001c47b8. Preserve its object input even though this error path ignores
 * it. No concrete dispatch establishes any additional unused inputs. */
unsigned func_001c457c(struct ResourceFile *file)
{
    return 0xe0c046f8u;
}
