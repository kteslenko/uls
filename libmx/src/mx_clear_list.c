#include "libmx.h"

void mx_clear_list(t_list **list) {
    t_list *current = *list;
    t_list *temp = NULL;

    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }

    *list = NULL;
}
