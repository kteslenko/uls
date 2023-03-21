char *mx_strrchr(const char *s, int c) {
    char *chr = 0;
    char *ptr = (char *)s;

    while (*ptr != '\0') {
        if (*ptr == c) {
            chr = ptr;
        }
        ptr++;
    }

    return chr;
}
