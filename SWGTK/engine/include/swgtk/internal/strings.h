#ifndef SWGTK_INTERNAL_STRINGS_H
#define SWGTK_INTERNAL_STRINGS_H

/*
    TODO: Implement the following functions *as needed*:
    
    explicit_bzero
    index
    rindex
    ffs
    ffsl
    ffsll
    strcasecmp
    strncasecmp
    strcasecmp_l
    strncasecmp_l

*/

#include <cstring>
inline int bcmp(const void *s1, const void *s2, size_t n) {
    return std::memcmp(s1, s2, n);
}

inline void bcopy(const void *src, void *dest, size_t n) {
    std::memcpy(dest, src, n);
}

inline void bzero(void* s, size_t n) {
    std::memset(s, 0, n);
}

#endif