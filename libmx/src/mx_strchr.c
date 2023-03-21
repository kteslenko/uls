char *mx_strchr(const char *s, int c) {
    char *ptr = (char*)s;

    while (*ptr != c
           && *ptr != '\0') {
        ptr++;
    }

    if (*ptr == c) {
        return ptr;
    }

    return 0;
}
