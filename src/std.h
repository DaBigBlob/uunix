#ifndef UUNIX_STD
#define UUNIX_STD

#include "pre.h"

#define memset(b, e, c) {\
    for (u8* _b=(u8*)(b); _b < (u8*)(e); ++_b) *_b = (u8)(c);\
}

#endif // UUNIX_STD
