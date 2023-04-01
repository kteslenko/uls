#include "output.h"

static void print_filetype(mode_t mode) {
    switch (mode & S_IFMT) {
    case S_IFBLK:
        mx_printchar('b');
        break;
    case S_IFCHR:
        mx_printchar('c');
        break;
    case S_IFDIR:
        mx_printchar('d');
        break;
    case S_IFIFO:
        mx_printchar('p');
        break;
    case S_IFLNK:
        mx_printchar('l');
        break;
    case S_IFSOCK:
        mx_printchar('s');
        break;
    default:
        mx_printchar('-');
        break;
    }
}

static void print_permissions(mode_t mode) {
    print_filetype(mode);
    mx_printchar((mode & S_IRUSR) ? 'r' : '-');
    mx_printchar((mode & S_IWUSR) ? 'w' : '-');
    if (mode & S_IXUSR) {
        mx_printchar((mode & S_ISUID) ? 's' : 'x');
    } else {
        mx_printchar((mode & S_ISUID) ? 'S' : '-');
    }
    mx_printchar((mode & S_IRGRP) ? 'r' : '-');
    mx_printchar((mode & S_IWGRP) ? 'w' : '-');
    if (mode & S_IXGRP) {
        mx_printchar((mode & S_ISGID) ? 's' : 'x');
    } else {
        mx_printchar((mode & S_ISGID) ? 'S' : '-');
    }
    mx_printchar((mode & S_IROTH) ? 'r' : '-');
    mx_printchar((mode & S_IWOTH) ? 'w' : '-');
    if (mode & S_IXOTH) {
        mx_printchar((mode & S_ISTXT) ? 't' : 'x');
    } else {
        mx_printchar((mode & S_ISTXT) ? 'T' : '-');
    }
}

static void print_aligned(char *string, int width, bool align_right) {
    int spaces = width - mx_strlen(string);

    if (!align_right) {
        mx_printstr(string);
    }
    for (int i = 0; i < spaces; i++) {
        mx_printchar(' ');
    }
    if (align_right) {
        mx_printstr(string);
    }
}

static void printlong_aligned(long long number, int width) {
    char *str = mx_ltoa(number);
    print_aligned(str, width, true);
    free(str);
}

static void print_time(time_t ptime, bool full) {
    char *str = ctime(&ptime);
    char **arr = mx_strsplit(str, ' ');
    time_t now = time(NULL);
    time_t six_months_sec = (365 / 2) * 24 * 60 * 60;

    arr[4][4] = '\0';
    if (full) {
        for (int i = 1; arr[i] != NULL; i++) {
            print_aligned(arr[i], 2, true);
            if (arr[i + 1] != NULL) {
                mx_printchar(' ');
            }
        }
    } else if (ptime + six_months_sec <= now || ptime >= now + six_months_sec) {
        mx_printstr(arr[1]);
        mx_printchar(' ');
        print_aligned(arr[2], 2, true);
        mx_printstr("  ");
        mx_printstr(arr[4]);
    } else {
        mx_printstr(arr[1]);
        mx_printchar(' ');
        print_aligned(arr[2], 2, true);
        mx_printchar(' ');
        char **arr_time = mx_strsplit(arr[3], ':');
        mx_printstr(arr_time[0]);
        mx_printchar(':');
        mx_printstr(arr_time[1]);
        mx_del_strarr(&arr_time);
    }
    mx_del_strarr(&arr);
}

static double round(double number) {
    return (long)(number + 0.5);
}

static void print_size(off_t size, int width) {
    const char *suffixes[] = {"B", "K", "M", "G", "T", "P"};
    double size_f = size;
    int suffix = 0;

    while (size_f >= 1000) {
        size_f /= 1024;
        suffix++;
    }

    double size_r = round(size_f * 10) / 10;
    char buf[5] = {'\0'};
    if (size_r >= 10 || suffix == 0) {
        char *str = mx_ltoa(round(size_f));
        mx_strcat(buf, str);
        free(str);
    } else {
        char *str = mx_ltoa(size_r);
        mx_strcat(buf, str);
        free(str);
        mx_strcat(buf, ".");
        str = mx_ltoa((long long)(size_r * 10) % 10);
        mx_strcat(buf, str);
        free(str);
    }
    mx_strcat(buf, suffixes[suffix]);
    print_aligned(buf, width, true);
}

static void print_xattrs(t_fileinfo *fileinfo, bool human_readable) {
    for (char **ptr = fileinfo->xattr_keys; *ptr != NULL; ptr++) {
        mx_printchar('\t');
        mx_printstr(*ptr);
        mx_printchar('\t');
        ssize_t value_size = getxattr(fileinfo->path, *ptr, NULL, 0, 0, XATTR_NOFOLLOW);
        if (human_readable) {
            print_size(value_size, 5);
        } else {
            printlong_aligned(value_size, 4);
        }
        mx_printstr(" \n");
    }
}

static void print_acl(acl_t acl) {
    char *str = acl_to_text(acl, NULL);
    char **lines = mx_strsplit(str, '\n');

    for (int i = 1; lines[i] != NULL; i++) {
        mx_printchar(' ');
        mx_printint(i - 1);
        mx_printstr(": ");
        char **parts = mx_strsplit(lines[i], ':');
        mx_printstr(parts[0]);
        mx_printchar(':');
        mx_printstr(parts[2]);
        mx_printchar(' ');
        mx_printstr(parts[4]);
        mx_printchar(' ');
        mx_printstr(parts[5]);
        mx_printchar('\n');
        mx_del_strarr(&parts);
    }
    mx_del_strarr(&lines);
    free(str);
}

static void print_fileinfo_long(t_fileinfo *fileinfo, t_width *width, t_config *config) {
    print_permissions(fileinfo->stat.st_mode);

    if (fileinfo->xattr_keys != NULL) {
        mx_printchar('@');
    } else if (fileinfo->acl != NULL) {
        mx_printchar('+');
    } else {
        mx_printchar(' ');
    }

    mx_printchar(' ');
    printlong_aligned(fileinfo->stat.st_nlink, width->links);
    mx_printchar(' ');

    if (!config->omit_owner) {
        print_aligned(fileinfo->user, width->user, false);
        mx_printstr("  ");
    }
    if (!config->omit_group) {
        print_aligned(fileinfo->group, width->group, false);
        mx_printstr("  ");
    }

    if (config->omit_owner && config->omit_group) {
        mx_printstr("  ");
    }

    if (S_ISCHR(fileinfo->stat.st_mode) || S_ISBLK(fileinfo->stat.st_mode)) {
        char *hex = mx_nbr_to_hex(fileinfo->stat.st_rdev);
        char *str;
        if (fileinfo->stat.st_rdev == 0) {
            str = mx_strdup(hex);
        } else {
            str = mx_strjoin("0x", hex);
        }
        print_aligned(str, width->size, true);
        free(str);
        free(hex);
    } else if (config->human_readable) {
        print_size(fileinfo->stat.st_size, width->size);
    } else {
        printlong_aligned(fileinfo->stat.st_size, width->size);
    }

    mx_printchar(' ');
    print_time(fileinfo->timespec.tv_sec, config->complete_time_info);
    mx_printchar(' ');
    print_fileinfo(fileinfo, config);
    if (fileinfo->link != NULL) {
        mx_printstr(" -> ");
        mx_printstr(fileinfo->link);
    }
    mx_printchar('\n');

    if (config->extended_attributes && fileinfo->xattr_keys != NULL) {
        print_xattrs(fileinfo, config->human_readable);
    }

    if (config->access_control_list && fileinfo->acl != NULL) {
        print_acl(fileinfo->acl);
    }
}

static t_width max_width(t_list *fileinfos, t_config *config) {
    t_width width = {.links = 0, .user = 0, .group = 0, .size = 0};

    while (fileinfos != NULL) {
        t_fileinfo *fileinfo = fileinfos->data;

        char *nlinks = mx_ltoa(fileinfo->stat.st_nlink);
        if (width.links < mx_strlen(nlinks)) {
            width.links = mx_strlen(nlinks);
        }
        free(nlinks);

        if (width.user < mx_strlen(fileinfo->user)) {
            width.user = mx_strlen(fileinfo->user);
        }

        if (width.group < mx_strlen(fileinfo->group)) {
            width.group = mx_strlen(fileinfo->group);
        }

        int size_len = 5;
        if (S_ISCHR(fileinfo->stat.st_mode) || S_ISBLK(fileinfo->stat.st_mode)) {
            char *wsize = mx_nbr_to_hex(fileinfo->stat.st_rdev);
            size_len = mx_strlen(wsize) + 2;
            free(wsize);
        } else if (!config->human_readable) {
            char *wsize = mx_ltoa(fileinfo->stat.st_size);
            size_len = mx_strlen(wsize);
            free(wsize);
        }
        if (width.size < size_len) {
            width.size = size_len;
        }

        fileinfos = fileinfos->next;
    }

    return width;
}

void print_long(t_list *fileinfos, t_config *config) {
    t_width width = max_width(fileinfos, config);

    while (fileinfos != NULL) {
        print_fileinfo_long(fileinfos->data, &width, config);
        fileinfos = fileinfos->next;
    }
}
