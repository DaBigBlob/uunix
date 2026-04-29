#ifndef UUNIX_STD
#define UUNIX_STD

#include "pre.h"

/* we would like to minimize stack use
 * (we dont have too much of it)
 */

/** Null sentinel iterator
Generic iterator over null terminated
memory blocks */
#define null_sentinel_foreach(begin, elem_t, id)                          \
    for (elem_t *id = (begin); *id; ++id)

/** Range iterator
Generic iterator over a range of memory
specified by (begin, exclusive_end) */
#define range_foreach(begin, end, elem_t, id)                             \
    for (elem_t *id = (elem_t *)(begin); id < (elem_t *)(end); ++id)

#define uint2cstr(size, _num, id)                                         \
    char id[] = STR(UINT##size##_MAX);                                    \
    do {                                                                  \
        char   *cur = id;                                                 \
        u##size num = _num;                                               \
        for (; *cur; ++cur)                                               \
            *cur = '0';                                                   \
        for (--cur; num && cur >= id; --cur) {                            \
            *cur = (num % 10) + 48;                                       \
            num /= 10;                                                    \
        }                                                                 \
    } while (0)

/** inrements pointer b by t till b = e,
for each b, *b = c of type t */
#define mem_set(b, e, t, c)                                               \
    do {                                                                  \
        range_foreach(b, e, t, _b) {                                      \
            *_b = c;                                                      \
        }                                                                 \
    } while (0)

/** inrements pointer b and d by u8 till b = e,
for each b, *b = *d of type t */
#define mem_cpy(b, e, t, d)                                               \
    do {                                                                  \
        t *_d = (t *)(d);                                                 \
        range_foreach(b, e, t, _b) {                                      \
            *_b = *_d;                                                    \
            ++_d;                                                         \
        }                                                                 \
    } while (0)

#endif // UUNIX_STD
