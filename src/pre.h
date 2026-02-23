#ifndef UUNIX_PRE
#define UUNIX_PRE

#if !defined(__STDC_VERSION__)
    #error "Fatal: __STDC_VERSION__ not defined."
#endif
#if __STDC_VERSION__ != 201710L
    /* I choose C17 because it is decently old, stable,
    and testedwith not new language features over C11
    and numerous defect fixes over C11. */
    #error "This is a C17-ONLY project."
#endif

#define MAX(a, b)               ((a) > (b) ? (a) : (b))
#define MIN(a, b)               ((a) < (b) ? (a) : (b))
#define CLAMP(a, lo, hi)        MIN(MAX(a, lo), hi)

#define STR(x) ___STR(x)
#define ___STR(x) #x

#define ROUNDUP(a, b)           ((((a)-1) / (b) + 1) * (b))
#define ROUNDDOWN(a, b)         ((a) / (b) * (b))

#define ARR_SIZE(x)             (sizeof(x) / sizeof((x)[0]))

#include <stdint.h> /* provided as per c17 clause 4 part 6 */
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

#include <stddef.h> /* provided as per c17 clause 4 part 6 */
typedef size_t                  usize;
typedef ptrdiff_t               isize;
typedef unsigned long           addr; /* virt & phy */

#include <stdbool.h> /* provided as per c17 clause 4 part 6 */
#define true                    1
#define false                   0

#define NORETURN                _Noreturn
#define ALIGNED(x)              _Alignas(n)

#define OFFSETOF(TYPE, MEMBER)  ((size_t) &((TYPE *)0)->MEMBER)
#define CONTNROF(ptr, type, member) (\
    (type *)((char *)(ptr) - OFFSETOF(type, member))\
)

/* these are some non C17-isms we are keeping */
#define PACKED                  __attribute__((packed))
#define ALWAYS_INLINE           inline __attribute__((always_inline))
#define LIKELY(x)               __builtin_expect((x), true)
#define UNLIKELY(x)             __builtin_expect((x), false)

#endif // UUNIX_PRE
