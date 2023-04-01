#pragma once

#include "config.h"
#include "libmx.h"

#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <sys/acl.h>
#include <sys/stat.h>
#include <sys/syslimits.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <unistd.h>

typedef struct {
    char *path;
    char *name;
    char *user;
    char *link;
    char *group;
    char **xattr_keys;
    acl_t acl;
    struct stat stat;
    struct timespec timespec;
} t_fileinfo;

t_fileinfo *get_fileinfo(const char *dir, const char *name, t_config *config);
bool get_dir_entries(t_list **entries, const char *name, t_config *config);

void free_fileinfo(t_fileinfo *fileinfo);

blkcnt_t count_blocks(t_list *files);
