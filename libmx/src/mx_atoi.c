#include "libmx.h"

static bool mx_isdigit(int c) {
    return c >= 48
        && c <= 57;
}

static bool mult_with_overflow(long *a, long b) {
    long result = 9223372036854775807 / b;

    if (result < *a) {
        return true;
    }

    *a *= b;
    return false;
}

static bool add_with_overflow(long *a, long b) {
    long result = 9223372036854775807 - b;

    if (result < *a) {
        return true;
    }

    *a += b;
    return false;
}

int mx_atoi(const char *str, int *out) {
    long result = 0;
    bool negative = false;

    if (*str == '-'
        || *str == '+') {
        if (*str == '-') {
            negative = true;
        }
        str++;
    }

    while (mx_isdigit(*str)) {
        if (mult_with_overflow(&result, 10)
            || add_with_overflow(&result, (*str - 48))) {
            *out = negative ? 0 : -1;
            return -1;
        }
        str++;
    }

    if (*str != '\0') {
        return -1;
    }

    *out = negative ? -result : result;
    return 0;
}
