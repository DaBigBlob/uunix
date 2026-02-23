#ifndef UUNIX_STD
#define UUNIX_STD

#include "pre.h"

#define memset(t, b, e, c) {\
    for (t _b=(t)(b); _b < (t)(e); ++_b) *_b = (u8)(c);\
}

#endif // UUNIX_STD
