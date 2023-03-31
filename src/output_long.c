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
    mx_printstr((mode & S_IRUSR) ? "r" : "-");
    mx_printstr((mode & S_IWUSR) ? "w" : "-");
    mx_printstr((mode & S_IXUSR) ? "x" : "-");
    mx_printstr((mode & S_IRGRP) ? "r" : "-");
    mx_printstr((mode & S_IWGRP) ? "w" : "-");
    mx_printstr((mode & S_IXGRP) ? "x" : "-");
    mx_printstr((mode & S_IROTH) ? "r" : "-");
    mx_printstr((mode & S_IWOTH) ? "w" : "-");
    mx_printstr((mode & S_IXOTH) ? "x" : "-");
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

static void printint_aligned(int number, int width) {
    char *str = mx_itoa(number);
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
        mx_printstr(" ");
        print_aligned(arr[2], 2, true);
        mx_printstr("  ");
        mx_printstr(arr[4]);
    } else {
        mx_printstr(arr[1]);
        mx_printstr(" ");
        print_aligned(arr[2], 2, true);
        mx_printstr(" ");
        char **arr_time = mx_strsplit(arr[3], ':');
        mx_printstr(arr_time[0]);
        mx_printchar(':');
        mx_printstr(arr_time[1]);
    }
}

static void print_xattrs(t_fileinfo *fileinfo) {
    for (char **ptr = fileinfo->xattr_keys; *ptr != NULL; ptr++) {
        mx_printstr("\t");
        mx_printstr(*ptr);
        mx_printstr("\t   ");
        ssize_t value_size = getxattr(fileinfo->path, *ptr, NULL, 0, 0, XATTR_NOFOLLOW);
        mx_printint(value_size);
        mx_printstr(" \n");
    }
}

static void print_acl(acl_t acl) {
    char *str = acl_to_text(acl, NULL);
    char **lines = mx_strsplit(str, '\n');

    for (int i = 1; lines[i] != NULL; i++) {
        mx_printstr(" ");
        mx_printint(i - 1);
        mx_printstr(": ");
        char **parts = mx_strsplit(lines[i], ':');
        mx_printstr(parts[0]);
        mx_printstr(":");
        mx_printstr(parts[2]);
        mx_printstr(" ");
        mx_printstr(parts[4]);
        mx_printstr(" ");
        mx_printstr(parts[5]);
        mx_printstr("\n");
        mx_del_strarr(&parts);
    }
    mx_del_strarr(&lines);
    free(str);
}

static double round(double number) {
    return (long)(number + 0.5);
}

static void print_size(off_t size) {
    const char *suffixes[] = {"B", "K", "M", "G", "T", "P"};
    double size_f = size;
    int suffix = 0;

    while (size_f >= 1000) {
        size_f /= 1024;
        suffix++;
    }

    size_f = round(size_f * 10) / 10;

    char buf[5] = {'\0'};
    if (size_f >= 10 || suffix == 0) {
        char *str = mx_itoa(round(size_f));
        mx_strcat(buf, str);
        free(str);
    } else {
        char *str = mx_itoa(size_f);
        mx_strcat(buf, str);
        free(str);
        mx_strcat(buf, ".");
        str = mx_itoa((int)(size_f * 10) % 10);
        mx_strcat(buf, str);
        free(str);
    }
    mx_strcat(buf, suffixes[suffix]);
    print_aligned(buf, 5, true);
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

    mx_printstr(" ");
    printint_aligned(fileinfo->stat.st_nlink, width->links);
    mx_printstr(" ");

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

    if (config->human_readable) {
        print_size(fileinfo->stat.st_size);
    } else {
        printint_aligned(fileinfo->stat.st_size, width->size);
    }

    mx_printstr(" ");
    print_time(fileinfo->timespec.tv_sec, config->complete_time_info);
    mx_printstr(" ");
    print_fileinfo(fileinfo, config);
    if (fileinfo->link != NULL) {
        mx_printstr(" -> ");
        mx_printstr(fileinfo->link);
    }
    mx_printstr("\n");

    if (config->extended_attributes && fileinfo->xattr_keys != NULL) {
        print_xattrs(fileinfo);
    }

    if (config->access_control_list && fileinfo->acl != NULL) {
        print_acl(fileinfo->acl);
    }
}

static t_width max_width(t_list *fileinfos) {
    t_width width = {.links = 0, .user = 0, .group = 0, .size = 0};

    while (fileinfos != NULL) {
        t_fileinfo *fileinfo = fileinfos->data;

        char *nlinks = mx_itoa(fileinfo->stat.st_nlink);
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

        char *wsize = mx_itoa(fileinfo->stat.st_size);
        if (width.size < mx_strlen(wsize)) {
            width.size = mx_strlen(wsize);
        }
        free(wsize);

        fileinfos = fileinfos->next;
    }

    return width;
}

void print_long(t_list *fileinfos, t_config *config) {
    t_width width = max_width(fileinfos);

    while (fileinfos != NULL) {
        print_fileinfo_long(fileinfos->data, &width, config);
        fileinfos = fileinfos->next;
    }
}
