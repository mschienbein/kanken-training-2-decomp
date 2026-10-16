/* Compiler service-call intrinsics, not inline assembly. These identify the
 * CTR service ABI, not the original assembly author's compiler or C credit. */
__svc(0x03) void exit_process_service(void);
__svc(0x09) int close_handle_service(unsigned int handle);
__svc(0x3c) void break_service(unsigned int reason);

void func_00100d6c(void) { exit_process_service(); }
int func_0012d148(unsigned int handle) { return close_handle_service(handle); }
void func_00134e7c(unsigned int reason) { break_service(reason); }
