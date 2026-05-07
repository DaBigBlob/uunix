#ifndef UUNIX_TASK
#define UUNIX_TASK
#include "pre.h" // IWYU pragma: keep
#include "hcb.h"

#define df0(a) u64 a
#define k      ,
noreturn void task_say_args(REGISTER_LIST_a(df0, k));
#undef k
#undef df0

noreturn void task_done(void);

#endif // UUNIX_TASK
