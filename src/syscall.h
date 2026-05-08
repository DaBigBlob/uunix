#ifndef UUNIX_SYSCALL
#define UUNIX_SYSCALL
#include "hcb.h"

#define SYS_MEM   0
#define SYS_READ  1
#define SYS_WRITE 2

void handle_syscall(volatile HCB *hcb);

#endif // UUNIX_SYSCALL
