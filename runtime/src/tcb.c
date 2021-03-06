#include <tcb.h>
#include <string.h>
#include <mm.h>
#include <io.h>
#include <syscall.h>

TCB* alloc_tcb(void) {
  TCB* tcb = mmap(0, STACK_SIZE, PROT_WRITE,
                  MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  if (MAP_FAILED == (void*)tcb) {
    // report_error
    quit(-1);
  }
  memset(tcb, 0, STACK_SIZE);
  /* create a guard page between the tcb and the thread-specific stack */
  if (0 != munmap((char*)tcb + PAGE_SIZE, PAGE_SIZE)) {
    // report_error
    quit(-1);
  }

  tcb->self = tcb;
  //tcb->canary = compute_canary();
  return tcb;
}

void dealloc_tcb(TCB *p) {
  munmap(p, STACK_SIZE);
}

void set_tcb_pointer(TCB *p) {
  if (0 != arch_prctl(ARCH_SET_FS, (unsigned long)p)) {
    report_error("[set_tcb] FS set failed\n");
  }
}
