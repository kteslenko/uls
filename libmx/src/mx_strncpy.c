char *mx_strncpy(char *dst, const char *src, int len) {
    int i;

    for (i = 0; i < len
         && src[i] != '\0'; i++) {
        dst[i] = src[i];
    }

    while (i < len) {
        dst[i] = '\0';
        i++;
    }

    return dst;
}
