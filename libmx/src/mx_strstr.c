#include "libmx.h"

char *mx_strstr(const char *s1, const char *s2) {
    int s2_len = mx_strlen(s2);

    if (s2_len == 0) {
        return (char*)s1;
    }

    s1 = mx_strchr(s1, *s2);
    while (s1 != 0) {
        if (mx_strncmp(s1, s2, s2_len) == 0) {
            return (char*)s1;
        }
        s1 = mx_strchr(s1 + 1, *s2);
    }

    return 0;
}
