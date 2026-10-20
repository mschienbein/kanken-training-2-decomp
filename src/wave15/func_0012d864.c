extern unsigned data_00242be4;

/* Both callers explicitly forward their descriptor pointer, although this
 * implementation reads the process-wide count instead. [INFERENCE] input role. */
unsigned func_0012d864(const void *descriptor)
{
    return data_00242be4;
}
