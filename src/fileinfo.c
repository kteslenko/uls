#include "fileinfo.h"

static char *get_user_name(uid_t uid, bool numeric) {
    struct passwd *passwd = getpwuid(uid);
    if (passwd == NULL || numeric) {
        return mx_itoa(uid);
    }
    return mx_strdup(passwd->pw_name);
}

static char *get_group_name(gid_t gid, bool numeric) {
    struct group *group = getgrgid(gid);
    if (group == NULL || numeric) {
        return mx_itoa(gid);
    }
    return mx_strdup(group->gr_name);
}

static struct timespec get_timespec(struct stat *stat, t_time_type time_type) {
    switch (time_type) {
    case TIME_MODIFICATION:
        return stat->st_mtimespec;
    case TIME_STATUS_CHANGE:
        return stat->st_ctimespec;
    case TIME_ACCESS:
        return stat->st_atimespec;
    case TIME_CREATION:
        return stat->st_birthtimespec;
    }
}

static char **get_xattr_keys(const char *filename) {
    char buffer[1024];
    ssize_t count = listxattr(filename, buffer, sizeof(buffer), XATTR_NOFOLLOW);

    for (int i = 0; i < count - 1; i++) {
        if (buffer[i] == '\0') {
            buffer[i] = ':';
        }
    }

    if (count != 0) {
        return mx_strsplit(buffer, ':');
    }
    return NULL;
}

t_fileinfo *get_fileinfo(const char *dir, const char *name, t_config *config, bool follow_link) {
    t_fileinfo *fileinfo = malloc(sizeof(t_fileinfo));

    fileinfo->path = mx_strjoin_delim(dir, name, '/');

    int err;
    if (follow_link) {
        err = stat(fileinfo->path, &fileinfo->stat);
    } else {
        err = lstat(fileinfo->path, &fileinfo->stat);
    }

    if (err != 0) {
        free(fileinfo);
        return NULL;
    }

    fileinfo->name = mx_strdup(name);
    fileinfo->user = get_user_name(fileinfo->stat.st_uid, config->display_numeric);
    fileinfo->group = get_group_name(fileinfo->stat.st_gid, config->display_numeric);
    if (S_ISLNK(fileinfo->stat.st_mode)) {
        fileinfo->link = malloc(PATH_MAX);
        readlink(fileinfo->path, fileinfo->link, PATH_MAX);
    } else {
        fileinfo->link = NULL;
    }
    fileinfo->timespec = get_timespec(&fileinfo->stat, config->time_type);
    fileinfo->xattr_keys = get_xattr_keys(fileinfo->path);
    fileinfo->acl = acl_get_file(fileinfo->path, ACL_TYPE_EXTENDED);

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

t_list *get_dir_entries(const char *name, t_config *config) {
    t_list *entries = NULL;
    DIR *dir = opendir(name);
    struct dirent *entry = NULL;

    while ((entry = readdir(dir)) != NULL) {
        if (!is_ignored(entry->d_name, config->ignore_type)) {
            mx_push_back(&entries, get_fileinfo(name, entry->d_name, config, false));
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
