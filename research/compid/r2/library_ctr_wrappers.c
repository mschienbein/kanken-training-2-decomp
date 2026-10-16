/* Public ABI identities checked against https://www.3dbrew.org/wiki/SVC.
 * Compiler service-call intrinsics test the wrappers without inline assembly.
 * The original no-frame stubs need not be compiler-generated C. */
__svc(0x03) void exit_process_service(void);
__svc(0x09) void exit_thread_service(void);
__svc(0x3c) void break_service(unsigned int reason);

void func_00100d6c(void) { exit_process_service(); }
void func_0012d148(void) { exit_thread_service(); }
void func_00134e7c(unsigned int reason) { break_service(reason); }
