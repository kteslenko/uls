#include "libmx.h"

static void swap(char **a, char **b) {
    char *temp = *a;
    *a = *b;
    *b = temp;
}

int mx_quicksort(char **arr, int left, int right) {
    int count = 0;
    int i = left;
    int j = right;
    char *pivot;

    if (arr == NULL)
        return -1;

    pivot = arr[(left + right) / 2];

    while (i <= j) {
        while (mx_strlen(arr[i]) < mx_strlen(pivot))
            i++;
        while (mx_strlen(arr[j]) > mx_strlen(pivot))
            j--;
        if (i <= j) {
            if (mx_strlen(arr[i]) != mx_strlen(arr[j])) {
                swap(&arr[i], &arr[j]);
                count++;
            }
            i++;
            j--;
        }
    }
    if (left < j)
        count += mx_quicksort(arr, left, j);
    if (i < right)
        count += mx_quicksort(arr, i, right);
    return count;
}
