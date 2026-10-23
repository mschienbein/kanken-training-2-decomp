#ifndef CTR_SVC_H
#define CTR_SVC_H

/* Raw ARM11 kernel calls, not the pointer-output userland wrapper API.
 * Names and semantic types: https://www.3dbrew.org/wiki/SVC
 * Register ABI: https://www.3dbrew.org/wiki/Kernel_ABI
 * Out-register marshaling: https://github.com/devkitPro/libctru/blob/master/libctru/source/svc.s
 *
 * ARMCC __svc accepts only r0-r3 arguments/results. A reserved_r0 argument
 * occupies the unused wrapper-output slot; it is not an output pointer.
 * __value_in_regs returns aggregate words consecutively from r0, without an
 * implicit result pointer. Callers marshal these results into userland objects.
 * ARMCC rejects long long arguments (#1502). SleepThread therefore takes
 * low/high words; WaitSynchronization1 below marshals a signed 64-bit timeout.
 *
 * ControlMemory (0x01), QueryMemory (0x02), CreateThread (0x08),
 * ArbitrateAddress (0x22) and WaitSynchronizationN (0x25) require r4/r5.
 * They deliberately have no misleading __svc declaration here: ARMCC rejects
 * their full raw ABIs (#1501 / #1505). Do not substitute wrapper signatures.
 */
typedef int CtrResult;
typedef unsigned int CtrHandle;
typedef unsigned int CtrMemoryPermission;
typedef int CtrResetType;
typedef unsigned int CtrProcessInfoType;
typedef unsigned int CtrBreakReason;

/* ARMCC's documented aligned type attribute expresses the unused r1 slot
 * before the wait timeout's low/high words in r2/r3. Keep the low word in
 * an object of this type: a cast alone is not a reliable parameter conversion
 * in the pinned compilers. This type is not a named-register binding.
 * https://developer.arm.com/documentation/dui0472/j/Compiler-specific-Features/--attribute----aligned---type-attribute
 */
typedef unsigned int CtrSvcTimeoutLow __attribute__((aligned(8)));

typedef struct CtrSvcHandleResult {
    CtrResult result;
    CtrHandle handle;
} CtrSvcHandleResult;

typedef struct CtrSvcProcessIdResult {
    CtrResult result;
    unsigned int process_id;
} CtrSvcProcessIdResult;

typedef struct CtrSvcProcessInfoResult {
    CtrResult result;
    unsigned int value_low;
    unsigned int value_high;
} CtrSvcProcessInfoResult;

#ifdef __cplusplus
extern "C" {
#endif

__svc(0x03) void svcExitProcess(void);
__svc(0x09) void svcExitThread(void);
__svc(0x0a) void svcSleepThread(unsigned int nanoseconds_low, int nanoseconds_high);
__svc(0x14) CtrResult svcReleaseMutex(CtrHandle mutex);
__svc(0x17) __value_in_regs CtrSvcHandleResult svcCreateEvent(unsigned int reserved_r0, CtrResetType reset_type);
__svc(0x18) CtrResult svcSignalEvent(CtrHandle event);
__svc(0x19) CtrResult svcClearEvent(CtrHandle event);
__svc(0x1e) __value_in_regs CtrSvcHandleResult svcCreateMemoryBlock(CtrMemoryPermission other_permission, unsigned int address, unsigned int size, CtrMemoryPermission my_permission);
__svc(0x1f) CtrResult svcMapMemoryBlock(CtrHandle memory_block, unsigned int address, CtrMemoryPermission my_permission, CtrMemoryPermission other_permission);
__svc(0x20) CtrResult svcUnmapMemoryBlock(CtrHandle memory_block, unsigned int address);
__svc(0x21) __value_in_regs CtrSvcHandleResult svcCreateAddressArbiter(void);
__svc(0x23) CtrResult svcCloseHandle(CtrHandle handle);
__svc(0x24) CtrResult ctrSvcWaitSynchronization1Raw(CtrHandle handle, CtrSvcTimeoutLow timeout_low, int timeout_high);
__svc(0x27) __value_in_regs CtrSvcHandleResult svcDuplicateHandle(unsigned int reserved_r0, CtrHandle original);
__svc(0x28) long long svcGetSystemTick(void);
__svc(0x2b) __value_in_regs CtrSvcProcessInfoResult svcGetProcessInfo(unsigned int reserved_r0, CtrHandle process, CtrProcessInfoType type);
__svc(0x2d) __value_in_regs CtrSvcHandleResult svcConnectToPort(unsigned int reserved_r0, const char *port_name);
__svc(0x32) CtrResult svcSendSyncRequest(CtrHandle session);
__svc(0x35) __value_in_regs CtrSvcProcessIdResult svcGetProcessId(unsigned int reserved_r0, CtrHandle process);
__svc(0x3c) void svcBreak(CtrBreakReason reason);

#ifdef __cplusplus
}
#endif

/* The fully initialized aligned low-word object retains the raw ABI's hole
 * without supplying any C value for the ignored r1 slot. Neither word is
 * discarded; signed right shift preserves the timeout's high 32 bits.
 */
static __forceinline CtrResult svcWaitSynchronization1(CtrHandle handle, long long nanoseconds)
{
    CtrSvcTimeoutLow timeout_low = (unsigned int)nanoseconds;
    int timeout_high = (int)(nanoseconds >> 32);
    return ctrSvcWaitSynchronization1Raw(handle, timeout_low, timeout_high);
}

#endif
