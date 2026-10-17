/* [INFERENCE] Element identity is unrecovered. Initializer 0x00214138
 * passes this callback to the array-constructor helper for sixty-four
 * eight-byte elements at bss_004b45d0. No member initialization occurs. */
struct ConstructorRecord001c4490 {
    unsigned int fields[2];
};

struct ConstructorRecord001c4490 *func_001c4490(struct ConstructorRecord001c4490 *object)
{
    return object;
}
