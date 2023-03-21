#include "libmx.h"

static int get_len(int number) {
    int len = 1;

    if (number < 0) {
        len++;
    }

    for (int i = number; i >= 10
         || i <= -10; i /= 10) {
        len++;
    }

    return len;
}

char *mx_itoa(int number) {
    int len = get_len(number);
    char *str = mx_strnew(len);
    int first = 0;

    if (number < 0) {
        str[0] = '-';
        first = 1;
    }
    for (int i = len; i > first; i--) {
        int digit = number % 10;

        if (digit < 0) {
            digit *= -1;
        }

        str[i - 1] = digit + 48;
        number /= 10;
    }

    return str;
}
