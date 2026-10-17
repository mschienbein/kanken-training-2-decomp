/* Caller func_00100d74 explicitly prepares r0 before the call at 0x00101338.
 * [INFERENCE] The unused word is an unsigned value; the adjacent callee does not read it. */

extern void func_0010e680(unsigned value);

void func_0010e67c(unsigned value)
{
    func_0010e680(value);
}
