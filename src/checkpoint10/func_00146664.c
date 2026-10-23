struct SharedMemoryBlock;
/* [INFERENCE] The caller forwards the block, backing address, byte size and
 * access flags; this wrapper supplies the final permission value. */
extern int func_00147f68(struct SharedMemoryBlock *block, void *address,
                       unsigned size, unsigned access, unsigned permissions);

int func_00146664(struct SharedMemoryBlock *block, void *address,
                  unsigned size, unsigned access)
{
    return func_00147f68(block, address, size, access, 3);
}
