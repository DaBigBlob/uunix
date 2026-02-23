#ifndef UUNIX_STD
#define UUNIX_STD

/*
    we would like to minimize stack use
    (we dont have too much of it)
 */

#define cstr_foreach(b, id, body) {\
    for (char *id=(b); *id; ++id) {body}\
}

#define mem_foreach(b, e, t, id, body) {\
    for (t *id=(b); id < (e); ++id) {body}\
}

/** inrements pointer b by t till b = e,
for each b, *b = c of type t */
#define mem_set(b, e, t, c) mem_foreach(b, e, t, _b, *_b = c;)

/** inrements pointer b and d by u8 till b = e,
for each b, *b = *d of type t */
#define mem_cpy(b, e, t, d) {\
    t *_d = (d);\
    mem_foreach(b, e, t, _b, {\
        *_b = *_d;\
        ++_d;\
    })\
}

#define uart_puts(urt, str) cstr_foreach(str, _c, uart_putc(urt, *_c))

#endif // UUNIX_STD
