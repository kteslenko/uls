#include "libmx.h"

int mx_count_substr(const char *str, const char *sub) {
    int count = 0;
    int sub_len;

    if (str == NULL
        || sub == NULL) {
        return -1;
    }

    sub_len = mx_strlen(sub);

    if (sub_len == 0) {
        return 0;
    }

    str = mx_strstr(str, sub);
    while (str != 0) {
        count++;
        str = mx_strstr(str + sub_len, sub);
    }

    return count;
}
