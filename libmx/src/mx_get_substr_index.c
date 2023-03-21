#include "libmx.h"

int mx_get_substr_index(const char *str, const char *sub) {
    char *ptr;
    
    if (str == NULL
        || sub == NULL) {
        return -2;
    }

    ptr = mx_strstr(str, sub);

    if (ptr == NULL) {
        return -1;
    }

    return ptr - str;
}
