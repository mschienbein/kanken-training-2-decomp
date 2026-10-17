#ifndef CTR_CP15_H
#define CTR_CP15_H

/* ARM11 TLS register and 0x200-byte per-thread block:
 * https://www.3dbrew.org/wiki/Thread_Local_Storage
 * The IPC command area is TLS + 0x80; static buffers begin at TLS + 0x180.
 * This is an ARMCC named coprocessor register declaration, not inline assembly.
 */
static __forceinline unsigned char *ctrGetThreadLocalStorage(void)
{
    register unsigned int tls __asm("cp15:0:c13:c0:3");
    return (unsigned char *)tls;
}

/* ARMv6 CP15 barrier operations (ARM DDI 0406, synchronization primitives).
 * https://developer.arm.com/documentation/ddi0406/c/Application-Level-Architecture/Application-Level-Memory-Model/Memory-access-order/Memory-barriers
 * __dmb/__dsb are not recognized as intrinsics by the pinned compiler for
 * --cpu=MPCore. Named CP15 writes express the ARM11 operations instead.
 * Schedule barriers also prevent compiler motion across each CPU barrier.
 */
static __forceinline void ctrDataMemoryBarrier(void)
{
    register volatile unsigned int barrier __asm("cp15:0:c7:c10:5");
    __schedule_barrier();
    barrier = 0;
    __schedule_barrier();
}

static __forceinline void ctrDataSynchronizationBarrier(void)
{
    register volatile unsigned int barrier __asm("cp15:0:c7:c10:4");
    __schedule_barrier();
    barrier = 0;
    __schedule_barrier();
}

#endif
