#include "libmx.h"

char *mx_del_extra_spaces(const char *str) {
    char *trimmed = mx_strtrim(str);
    char *r_ptr = trimmed;
    char *w_ptr = trimmed;
    char *new_str;

    if (trimmed == NULL) {
        return NULL;
    }

    while (*r_ptr != '\0')
    {
        if (!mx_isspace(*r_ptr)) {
            if (r_ptr != trimmed
                && mx_isspace(*(r_ptr - 1))) {
                *w_ptr = ' ';
                w_ptr++;
            }
            *w_ptr = *r_ptr;
            w_ptr++;
        }

        r_ptr++;
    }

    new_str = mx_strnew(w_ptr - trimmed);
    mx_strncpy(new_str, trimmed, w_ptr - trimmed);
    mx_strdel(&trimmed);
    return new_str;
}
