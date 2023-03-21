#include "libmx.h"

static void swap_data(t_list *a, t_list *b) {
    void *temp = a->data;

    a->data = b->data;
    b->data = temp;
}

t_list *mx_sort_list(t_list *list, bool (*cmp)(void *a, void *b)) {
    bool sorted = false;

    if (list == NULL || cmp == NULL) {
        return list;
    }

    while (!sorted) {
        sorted = true;
        for (t_list *i = list; i->next != NULL; i = i->next) {
            if (cmp(i->data, i->next->data)) {
                swap_data(i, i->next);
                sorted = false;
            }
        }
    }

    return list;
}
