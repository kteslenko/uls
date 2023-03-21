#include "libmx.h"

void *mx_realloc(void *ptr, size_t size) {
    size_t prev_size;
    void *new_ptr;

    if (ptr == NULL) {
        return malloc(size);
    }
    
    prev_size = malloc_size(ptr);
    if (prev_size >= size) {
        return ptr;
    }

    new_ptr = mx_memcpy(malloc(size), ptr, prev_size);
    free(ptr);
    return new_ptr;
}
