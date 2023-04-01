#include "output.h"

static void print_color(mode_t mode) {
    switch (mode & S_IFMT) {
    case S_IFBLK:
        mx_printstr("\033[34;46m");
        break;
    case S_IFCHR:
        mx_printstr("\033[34;43m");
        break;
    case S_IFDIR:
        if (mode & S_IWOTH) {
            if (mode & S_ISTXT) {
                mx_printstr("\033[30;42m");
            } else {
                mx_printstr("\033[30;43m");
            }
        } else {
            mx_printstr("\033[34m");
        }
        break;
    case S_IFIFO:
        mx_printstr("\033[33m");
        break;
    case S_IFLNK:
        mx_printstr("\033[35m");
        break;
    case S_IFSOCK:
        mx_printstr("\033[32m");
        break;
    default:
        if (mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
            if (mode & S_ISUID) {
                mx_printstr("\033[30;41m");
            } else if (mode & S_ISGID) {
                mx_printstr("\033[30;46m");
            } else {
                mx_printstr("\033[31m");
            }
        }
        break;
    }
}

static int print_classifier(mode_t mode, bool slash_only) {
    char classifier = '\0';

    switch (mode & S_IFMT) {
    case S_IFDIR:
        classifier = '/';
        break;
    case S_IFIFO:
        classifier = '|';
        break;
    case S_IFLNK:
        classifier = '@';
        break;
    case S_IFSOCK:
        classifier = '=';
        break;
    case S_IFREG:
        if (mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
            classifier = '*';
        }
        break;
    }

    if (classifier != '\0') {
        if (slash_only && classifier != '/') {
            return 0;
        }
        mx_printchar(classifier);
        return 1;
    }
    return 0;
}

static char *substitute_non_printable(const char *name) {
    char *copy = mx_strdup(name);

    for (int i = 0; copy[i] != '\0'; i++) {
        if (copy[i] >= 0 && copy[i] <= 31) {
            copy[i] = '?';
        }
    }

    return copy;
}

static void print_name(const char *name, bool non_printable) {
    if (non_printable) {
        char *str = substitute_non_printable(name);
        mx_printstr(str);
        free(str);
    } else {
        mx_printstr(name);
    }
}

int print_fileinfo(t_fileinfo *fileinfo, t_config *config) {
    if (config->colorize) {
        print_color(fileinfo->stat.st_mode);
    }
    print_name(fileinfo->name, config->non_printable);
    if (config->colorize) {
        mx_printstr("\033[0m");
    }

    int count = mx_strlen(fileinfo->name);

    if (config->classify || config->slash) {
        count += print_classifier(fileinfo->stat.st_mode, config->slash);
    }

    return count;
}

void print_singlecolumn(t_list *fileinfos, t_config *config) {
    while (fileinfos != NULL) {
        print_fileinfo(fileinfos->data, config);
        mx_printchar('\n');
        fileinfos = fileinfos->next;
    }
}

void print_stream(t_list *fileinfos, t_config *config) {
    while (fileinfos != NULL) {
        print_fileinfo(fileinfos->data, config);
        if (fileinfos->next != NULL) {
            mx_printstr(", ");
        }
        fileinfos = fileinfos->next;
    }
    mx_printchar('\n');
}

void print_fileinfos(t_list *fileinfos, t_config *config) {
    switch (config->format) {
    case FORMAT_SINGLECOLUMN:
        print_singlecolumn(fileinfos, config);
        break;
    case FORMAT_MULTICOLUMN:
        print_multicolumn(fileinfos, config);
        break;
    case FORMAT_STREAM:
        print_stream(fileinfos, config);
        break;
    case FORMAT_LONG:
        print_long(fileinfos, config);
        break;
    default:
        break;
    }
}
