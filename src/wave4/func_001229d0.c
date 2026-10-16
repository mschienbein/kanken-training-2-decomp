/* Signature: the callee stores r0 into context slot r1 (< 7), which the event worker
 * func_00137984 later calls through BLX, so r0 is a callback (review c004 F04). */

/* [INFERENCE] callback signature: the event worker func_00137984 invokes the slot via BLX. */
typedef void (*EventCallback)(void);

extern void func_001229d4(EventCallback callback, unsigned index);

void func_001229d0(EventCallback callback, unsigned index)
{
    func_001229d4(callback, index);
}
