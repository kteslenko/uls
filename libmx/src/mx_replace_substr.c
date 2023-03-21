#include "libmx.h"

char *mx_replace_substr(const char *str, const char *sub, const char *replace) {
    int count;
    char *new_str;
    char *ptr;
    char *sub_ptr;

    if (str == NULL ||
        sub == NULL ||
        replace == NULL) {
        return NULL;
    }

    count = mx_count_substr(str, sub);
    new_str = mx_strnew(mx_strlen(str) - mx_strlen(sub) * count + mx_strlen(replace) * count);
    ptr = new_str;

    sub_ptr = mx_strstr(str, sub);
    while (sub_ptr != NULL)
    {
        mx_strncpy(ptr, str, sub_ptr - str);
        ptr += sub_ptr - str;
        str = sub_ptr + mx_strlen(sub);
        mx_strcat(ptr, replace);
        ptr += mx_strlen(replace);
        sub_ptr = mx_strstr(str, sub);
    }
    mx_strcat(ptr, str);
    
    return new_str;
}
