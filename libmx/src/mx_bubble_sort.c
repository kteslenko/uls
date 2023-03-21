#include "libmx.h"

static void swap(char **a, char **b) {
    char* temp = *a;

    *a = *b;
    *b = temp;
}

int mx_bubble_sort(char **arr, int size) {
    int sorted = 0;
    int swaps = 0;

    while (!sorted) {
        sorted = 1;
        for (int i = 1; i < size; i++) {
            if (mx_strcmp(arr[i - 1], arr[i]) > 0) {
                swap(&arr[i - 1], &arr[i]);
                sorted = 0;
                swaps++;
            }
        }
    }

    return swaps;
}
