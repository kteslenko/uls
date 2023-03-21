#include "libmx.h"

static const char *find_word(const char *s, char c) {
    while (*s == c
           && *s != '\0') {
        s++;
    }

    if (*s == '\0') {
        return 0;
    }

    return s;
}

static int word_length(const char *s, char c) {
    int length = 0;

    while (s[length] != c
           && s[length] != '\0') {
        length++;
    }

    return length;
}

char **mx_strsplit(const char *s, char c) {
    int count;
    char **arr;
    const char *word;

    if (s == NULL) {
        return NULL;
    }

    count = mx_count_words(s, c);
    arr = malloc((count + 1) * sizeof(char*));
    arr[count] = NULL;

    word = find_word(s, c);
    for (int i = 0; word != 0; i++) {
        int len = word_length(word, c);

        arr[i] = mx_strnew(len);
        mx_strncpy(arr[i], word, len);
        word = find_word(word + len, c);
    }

    return arr;
}
