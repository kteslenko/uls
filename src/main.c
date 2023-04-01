#include "config.h"
#include "fileinfo.h"
#include "libmx.h"
#include "output.h"
#include "sort.h"

#include <errno.h>
#include <string.h>

static t_list *get_files_list(int argc, char *argv[]) {
    t_list *files = NULL;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-' || files != NULL) {
            mx_push_back(&files, argv[i]);
        }
    }

    if (files == NULL) {
        mx_push_back(&files, ".");
    }
    return files;
}

static void print_error(const char *file) {
    mx_printerr("uls: ");
    mx_printerr(file);
    mx_printerr(": ");
    mx_printerr(strerror(errno));
    mx_printerr("\n");
}

static void free_fileinfos(t_list *fileinfos) {
    t_list *temp = fileinfos;

    while (temp != NULL) {
        t_fileinfo *fileinfo = temp->data;
        free_fileinfo(fileinfo);
        temp = temp->next;
    }

    mx_clear_list(&fileinfos);
}

static void list_regular(t_list *regular, t_config *config) {
    if (regular != NULL) {
        sort_fileinfos(regular, config->sort_type, config->sort_reverse);
        print_fileinfos(regular, config);
    }
}

static bool list_dirs(t_list *dirs, t_config *config, bool print_names) {
    bool ret = true;

    sort_fileinfos(dirs, config->sort_type, config->sort_reverse);

    while (dirs != NULL) {
        t_fileinfo *fileinfo = dirs->data;
        t_list *fileinfos = NULL;
        bool status;

        dirs = dirs->next;

        if (print_names) {
            mx_printstr(fileinfo->name);
            mx_printstr(":\n");
        }

        status = get_dir_entries(&fileinfos, fileinfo->name, config);

        if (config->format == FORMAT_LONG) {
            mx_printstr("total ");
            mx_printint(count_blocks(fileinfos));
            mx_printchar('\n');
        }

        if (!status) {
            print_error(fileinfo->name);
            if (dirs != NULL) {
                mx_printchar('\n');
            }
            ret = false;
            continue;
        }

        sort_fileinfos(fileinfos, config->sort_type, config->sort_reverse);
        print_fileinfos(fileinfos, config);

        free_fileinfos(fileinfos);

        if (dirs != NULL) {
            mx_printchar('\n');
        }
    }

    return ret;
}

static int ls(t_list *files, t_config *config) {
    t_list *regular = NULL;
    t_list *dirs = NULL;
    bool print_names = mx_list_size(files) > 1;
    int status = 0;

    sort_filenames(files, config->sort_type);
    while (files != NULL) {
        char *file = files->data;
        files = files->next;

        t_fileinfo *fileinfo = get_fileinfo(NULL, file, config);
        if (fileinfo == NULL) {
            print_error(file);
            status = 1;
            continue;
        }

        if (S_ISDIR(fileinfo->stat.st_mode)) {
            mx_push_back(&dirs, fileinfo);
        } else {
            mx_push_back(&regular, fileinfo);
        }
    }

    list_regular(regular, config);
    if (regular != NULL && dirs != NULL) {
        mx_printchar('\n');
    }
    if (!list_dirs(dirs, config, print_names)) {
        status = 1;
    }
    free_fileinfos(regular);
    free_fileinfos(dirs);
    return status;
}

int main(int argc, char *argv[]) {
    t_list *files = get_files_list(argc, argv);
    t_config *config = parse_args(argc, argv);

    int status = ls(files, config);

    mx_clear_list(&files);
    free(config);
    return status;
}
