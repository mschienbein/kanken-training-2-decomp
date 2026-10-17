/* Array-element callback: func_001ef960 supplies this to func_001911cc,
 * which calls it with each writable 0xd4-byte element in r0.
 * [INFERENCE] The return is void: the array helper ignores the callback result;
 * the return-only body cannot distinguish void from pointer identity. */
void func_001c0e44(void *element)
{
}
