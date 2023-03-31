#include "sort.h"

static bool compare_strings(void *data1, void *data2) {
    return mx_strcmp(data1, data2) > 0;
}

void sort_filenames(t_list *filenames, t_sort_type sort_type) {
    if (sort_type != SORT_NONE) {
        mx_sort_list(filenames, compare_strings, false);
    }
}

static bool compare_by_name(void *data1, void *data2) {
    t_fileinfo *fileinfo1 = data1;
    t_fileinfo *fileinfo2 = data2;
    return mx_strcmp(fileinfo1->name, fileinfo2->name) > 0;
}

static bool compare_by_time(void *data1, void *data2) {
    t_fileinfo *fileinfo1 = data1;
    t_fileinfo *fileinfo2 = data2;
    if (fileinfo1->timespec.tv_sec == fileinfo2->timespec.tv_sec) {
        if (fileinfo1->timespec.tv_nsec == fileinfo2->timespec.tv_nsec) {
            return mx_strcmp(fileinfo1->name, fileinfo2->name) > 0;
        }
        return fileinfo1->timespec.tv_nsec < fileinfo2->timespec.tv_nsec;
    }
    return fileinfo1->timespec.tv_sec < fileinfo2->timespec.tv_sec;
}

static bool compare_by_size(void *data1, void *data2) {
    t_fileinfo *fileinfo1 = data1;
    t_fileinfo *fileinfo2 = data2;
    if (fileinfo1->stat.st_size == fileinfo2->stat.st_size) {
        return mx_strcmp(fileinfo1->name, fileinfo2->name) > 0;
    }
    return fileinfo1->stat.st_size < fileinfo2->stat.st_size;
}

void sort_fileinfos(t_list *fileinfos, t_sort_type sort_type, bool reverse) {
    switch (sort_type) {
    case SORT_NAME:
        mx_sort_list(fileinfos, compare_by_name, reverse);
        break;
    case SORT_TIME:
        mx_sort_list(fileinfos, compare_by_time, reverse);
        break;
    case SORT_SIZE:
        mx_sort_list(fileinfos, compare_by_size, reverse);
        break;
    default:
        break;
    }
}
