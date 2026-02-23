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

typedef signed char             i8;
typedef unsigned char           u8;
typedef signed short                   i16;
typedef unsigned short          u16;
typedef int                     i32;
typedef unsigned int            u32;
typedef long                    i64;
typedef unsigned long           u64;

typedef unsigned long           usize;
typedef long                    isize;
typedef unsigned long           addr; /* virt & phy */

typedef char                    bool;
#define true                    1
#define false                   0

#define NULL                    ((void *)0)

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
