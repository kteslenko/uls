#include "libmx.h"

char *mx_strndup(const char *str, size_t n) {
    size_t len = mx_strlen(str);

    if (n < len) {
        len = n;
    }

    return mx_strncpy(mx_strnew(len), str, len);
}
