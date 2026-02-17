#ifndef UUNIX_TYPE
#define UUNIX_TYPE

typedef signed char             s8;
typedef unsigned char           u8;
typedef short                   i16;
typedef unsigned short          u16;
typedef int                     i32;
typedef unsigned int            u32;
#if __riscv_xlen == 64
    typedef long                i64;
    typedef unsigned long       u64;
#elif __riscv_xlen == 32
    /* we dont really support 32bit */
    typedef long long           i64;
    typedef unsigned long long  u64;
#endif
typedef unsigned long           usize;
typedef long                    isize;
typedef unsigned long           addr; /* virt & phy */

#if __STDC_VERSION__ < 202000L
    /* those mfs added bool in c2x */
    typedef _Bool               bool;
    #define true                1
    #define false               0
#endif

#define NULL                    ((void *)0)

#define __packed                __attribute__((packed))
#define __noreturn              __attribute__((noreturn))
#define __aligned(x)            __attribute__((aligned(x)))

#ifndef __always_inline
    #define __always_inline     inline __attribute__((always_inline))
#endif
#define INLINE __always_inline

#define likely(x)               __builtin_expect((x), 1)
#define unlikely(x)             __builtin_expect((x), 0)

#ifndef __has_builtin
    #define __has_builtin(...)  0
    /* default to false if DNE */
#endif

#ifdef offsetof
    #undef offsetof
#endif

#if __has_builtin(__builtin_offsetof)
    #define offsetof(TYPE, MEMBER) __builtin_offsetof(TYPE,MEMBER)
#elif defined(__compiler_offsetof)
    #define offsetof(TYPE, MEMBER) __compiler_offsetof(TYPE,MEMBER)
#else
    #define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#define container_of(ptr, type, member) (\
    (type *)((char *)(ptr) - offsetof(type, member))\
)

#define array_size(x)           (sizeof(x) / sizeof((x)[0]))

#define MAX(a, b)               ((a) > (b) ? (a) : (b))
#define MIN(a, b)               ((a) < (b) ? (a) : (b))
#define CLAMP(a, lo, hi)        MIN(MAX(a, lo), hi)

#define STR(x) ___STR(x)
#define ___STR(x) #x

#define ROUNDUP(a, b)           ((((a)-1) / (b) + 1) * (b))
#define ROUNDDOWN(a, b)         ((a) / (b) * (b))

#endif // UUNIX_TYPE
