#include "libmx.h"

int mx_list_size(t_list *list) {
    int count = 0;

    while (list != NULL) {
        list = list->next;
        count++;
    }

    return count;
}
