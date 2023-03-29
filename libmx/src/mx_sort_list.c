#include "libmx.h"

static void swap_data(t_list *a, t_list *b) {
    void *temp = a->data;

    a->data = b->data;
    b->data = temp;
}

t_list *mx_sort_list(t_list *list, bool (*cmp)(void *, void *), bool reverse) {
    bool sorted = false;

    if (list == NULL || cmp == NULL) {
        return list;
    }

    while (!sorted) {
        sorted = true;
        for (t_list *i = list; i->next != NULL; i = i->next) {
            bool cmp_res = cmp(i->data, i->next->data);
            if ((cmp_res && !reverse) || (!cmp_res && reverse)) {
                swap_data(i, i->next);
                sorted = false;
            }
        }
    }

    return list;
}
