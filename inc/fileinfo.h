#pragma once

#include "config.h"
#include "libmx.h"

#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <sys/stat.h>

typedef struct {
    char *name;
    char *user;
    char *group;
    struct stat stat;
    struct timespec timespec;
} t_fileinfo;

t_fileinfo *get_fileinfo(const char *dir, const char *name, t_config *config);
t_list *get_dir_entries(const char *name, t_config *config);

blkcnt_t count_blocks(t_list *files);
