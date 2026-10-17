/* Signature: the callee uses r0-r3 as screen/table base, slot index, stored value and a nullable
 * UTF-16 text pointer (null selects a default string; review c004 F03). All four are forwarded. */

extern void func_001c3ac0(void *screen, int index, unsigned value, const unsigned short *text);

void func_001c3abc(void *screen, int index, unsigned value, const unsigned short *text)
{
    func_001c3ac0(screen, index, value, text); /* text may be null: callee substitutes a default */
}
