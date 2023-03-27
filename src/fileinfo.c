#include "fileinfo.h"

static char *filename(const char *path) {
    char *slash = mx_strrchr(path, '/');

    if (slash == NULL) {
        return mx_strdup(path);
    }
    return mx_strdup(slash + 1);
}

static char *get_user_name(uid_t uid) {
    struct passwd *passwd = getpwuid(uid);
    if (passwd == NULL) {
        return mx_itoa(uid);
    }
    return mx_strdup(passwd->pw_name);
}

static char *get_group_name(gid_t gid) {
    struct group *group = getgrgid(gid);
    if (group == NULL) {
        return mx_itoa(gid);
    }
    return mx_strdup(group->gr_name);
}

t_fileinfo *get_fileinfo(const char *name) {
    t_fileinfo *fileinfo = malloc(sizeof(t_fileinfo));

    lstat(name, &fileinfo->stat);
    fileinfo->name = filename(name);
    fileinfo->user = get_user_name(fileinfo->stat.st_uid);
    fileinfo->group = get_group_name(fileinfo->stat.st_gid);

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
            char *filename = mx_strjoin_delim(name, entry->d_name, '/');
            mx_push_back(&entries, get_fileinfo(filename));
            free(filename);
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
