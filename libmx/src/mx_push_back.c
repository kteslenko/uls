#include "libmx.h"

void mx_push_back(t_list **list, void *data) {
    t_list *current = *list;
    t_list *node = mx_create_node(data);

    if (current == NULL) {
        *list = node;
        return;
    }

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = node;
}
