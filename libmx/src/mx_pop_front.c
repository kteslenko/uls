#include "libmx.h"

void mx_pop_front(t_list **list) {
    t_list *temp;

    if (list == NULL || *list == NULL) {
        return;
    }

    temp = *list;
    *list = temp->next;
    free(temp);
}
