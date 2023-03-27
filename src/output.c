#include "output.h"

static void print_fileinfo(t_fileinfo *fileinfo) {
    mx_printstr(fileinfo->name);
}

static void print_permissions(mode_t mode) {
    mx_printstr((S_ISDIR(mode)) ? "d" : "-");
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

static void print_time(time_t time) {
    char *str = ctime(&time);
    str[16] = '\0';
    mx_printstr(str + 4);
}

static void print_fileinfo_long(t_fileinfo *fileinfo) {
    print_permissions(fileinfo->stat.st_mode);
    mx_printstr("  ");
    mx_printint(fileinfo->stat.st_nlink);
    mx_printstr(" ");
    mx_printstr(fileinfo->user);
    mx_printstr("  ");
    mx_printstr(fileinfo->group);
    mx_printstr("   ");
    mx_printint(fileinfo->stat.st_size);
    mx_printstr(" ");
    print_time(fileinfo->stat.st_mtimespec.tv_sec);
    mx_printstr(" ");
    mx_printstr(fileinfo->name);
}

static void print_singlecolumn(t_list *fileinfos) {
    while (fileinfos != NULL) {
        print_fileinfo(fileinfos->data);
        mx_printchar('\n');
        fileinfos = fileinfos->next;
    }
}

static void print_long(t_list *fileinfos) {
    mx_printstr("total ");
    mx_printint(count_blocks(fileinfos));
    mx_printstr("\n");
    while (fileinfos != NULL) {
        print_fileinfo_long(fileinfos->data);
        mx_printchar('\n');
        fileinfos = fileinfos->next;
    }
}

void print_fileinfos(t_list *fileinfos, t_config *config) {
    switch (config->format) {
    case FORMAT_SINGLECOLUMN:
        print_singlecolumn(fileinfos);
        break;
    case FORMAT_LONG:
        print_long(fileinfos);
        break;
    default:
        mx_printstr("go away");
        break;
    }
}
