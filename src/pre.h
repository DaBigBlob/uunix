#ifndef UUNIX_PRE
#define UUNIX_PRE

#if !defined(__STDC_VERSION__)
    #error "__STDC_VERSION__ not defined. Unacceptable."
#endif
#if __STDC_VERSION__ != 201710L
    /* I choose C17 because it is decently old, stable,
    and testedwith not new language features over C11
    and numerous defect fixes over C11. */
    #error "This is a C17-ONLY project."
#endif

/* provided as per c17 clause 4 part 6 */
#include <float.h>
#include <iso646.h>
#include <limits.h>
#include <stdalign.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX(a, b)               ((a) > (b) ? (a) : (b))
#define MIN(a, b)               ((a) < (b) ? (a) : (b))
#define CLAMP(a, lo, hi)        MIN(MAX(a, lo), hi)

#define STR(x) ___STR(x)
#define ___STR(x) #x

#define ROUNDUP(a, b)           ((((a)-1) / (b) + 1) * (b))
#define ROUNDDOWN(a, b)         ((a) / (b) * (b))

#define ARR_SIZE(x)             (sizeof(x) / sizeof((x)[0]))

#define DEF_SIZE_ALIAS(n)\
    typedef int##n##_t i##n;\
    typedef uint##n##_t u##n;
DEF_SIZE_ALIAS(8)
DEF_SIZE_ALIAS(16)
DEF_SIZE_ALIAS(32)
DEF_SIZE_ALIAS(64)
#undef DEF_SIZE_ALIAS

typedef float                   f32;
typedef double                  f64;

typedef size_t                  usize;
typedef ptrdiff_t               isize;
typedef size_t                  addr; /* virt & phy */

#define containerof(ptr, type, member) (\
    (type *)((char *)(ptr) - offsetof(type, member))\
)

#endif // UUNIX_PRE
