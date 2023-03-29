#include "libmx.h"

char *mx_strjoin_delim(const char *s1, const char *s2, char delim) {
    if (s1 == NULL) {
        return mx_strdup(s2);
    }

    int len1 = mx_strlen(s1);
    int len2 = mx_strlen(s2);
    char *str = mx_strnew(len1 + len2 + 1);

    mx_strcpy(str, s1);
    str[len1] = delim;
    mx_strcpy(str + len1 + 1, s2);

    return str;
}
