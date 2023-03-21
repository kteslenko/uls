#include "libmx.h"

void *mx_memmem(const void *big, size_t big_len, const void *little, size_t little_len) {
    void *ptr;

    if (little_len == 0) {
        return NULL;
    }

    ptr = mx_memchr(big, *(unsigned char*)little, big_len);
    while (ptr != NULL) {
        size_t remaining = big_len - ((unsigned char*)ptr - (unsigned char*)big);

        if (remaining < little_len) {
            break;
        }
        if (mx_memcmp(ptr, little, little_len) == 0) {
            return ptr;
        }

        ptr = mx_memchr((unsigned char*)ptr + 1, *(unsigned char*)little, big_len);
    }

    return NULL;
}
