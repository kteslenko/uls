#include "libmx.h"

char *mx_strdup(const char *str) {
    char *new_str = mx_strnew(mx_strlen(str));

    return mx_strcpy(new_str, str);
}
