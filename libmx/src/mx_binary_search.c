#include "libmx.h"

int mx_binary_search(char **arr, int size, const char *s, int *count) {
    int left = 0;
    int right = size - 1;

    *count = 0;
    while (left <= right)
    {
        int mid = (left + right) / 2;
        int cmp = mx_strcmp(s, arr[mid]);

        (*count)++;
        if (cmp == 0) {
            return mid;
        }
        if (cmp > 0) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    *count = 0;
    return -1;
}
