#include "fileinfo.h"

#include <dirent.h>

t_fileinfo *get_fileinfo(const char *name) {
    t_fileinfo *fileinfo = malloc(sizeof(t_fileinfo));

    fileinfo->name = mx_strdup(name);
    stat(name, &fileinfo->stat);

    return fileinfo;
}

static bool is_ignored(const char *name, t_ignore_type ignore_type) {
    if (ignore_type == IGNORE_HIDDEN && name[0] == '.') {
        return true;
    }
    if (ignore_type == IGNORE_DOTS && (mx_strcmp(name, ".") == 0 || mx_strcmp(name, "..") == 0)) {
        return true;
    }
    return false;
}

t_list *get_dir_entries(const char *name, t_ignore_type ignore_type) {
    t_list *entries = NULL;
    DIR *dir = opendir(name);
    struct dirent *entry = NULL;

    while ((entry = readdir(dir)) != NULL) {
        if (!is_ignored(entry->d_name, ignore_type)) {
            mx_push_back(&entries, get_fileinfo(entry->d_name));
        }
    }

    closedir(dir);
    return entries;
}

blkcnt_t count_blocks(t_list *files) {
    t_list *current = files;
    blkcnt_t blocks = 0;

    while (current != NULL) {
        t_fileinfo *fileinfo = current->data;

        blocks += fileinfo->stat.st_blocks;
        current = current->next;
    }

    return blocks;
}
