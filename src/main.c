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

static void list_regular(t_list *regular, t_config *config) {
    if (regular != NULL) {
        sort_fileinfos(regular, config->sort_type, config->sort_reverse);
        print_fileinfos(regular, config);
    }
}

static void list_dirs(t_list *dirs, t_config *config, bool print_names) {
    sort_fileinfos(dirs, config->sort_type, config->sort_reverse);

    while (dirs != NULL) {
        t_fileinfo *fileinfo = dirs->data;
        t_list *fileinfos = get_dir_entries(fileinfo->name, config);

        if (print_names) {
            mx_printstr(fileinfo->name);
            mx_printstr(":\n");
        }

        if (config->format == FORMAT_LONG) {
            mx_printstr("total ");
            mx_printint(count_blocks(fileinfos));
            mx_printstr("\n");
        }

        sort_fileinfos(fileinfos, config->sort_type, config->sort_reverse);
        print_fileinfos(fileinfos, config);

        if (dirs->next != NULL) {
            mx_printstr("\n");
        }
        dirs = dirs->next;
    }
}

static void ls(t_list *files, t_config *config) {
    t_list *regular = NULL;
    t_list *dirs = NULL;
    bool print_names = mx_list_size(files) > 1;
    bool follow_link = config->format != FORMAT_LONG && !config->colorize && !config->classify;

    sort_filenames(files, config->sort_type);
    while (files != NULL) {
        char *file = files->data;
        files = files->next;

        t_fileinfo *fileinfo = get_fileinfo(NULL, file, config, follow_link);
        if (fileinfo == NULL) {
            print_error(file);
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
        mx_printstr("\n");
    }
    list_dirs(dirs, config, print_names);
}

int main(int argc, char *argv[]) {
    t_list *files = get_files_list(argc, argv);
    t_config *config = parse_args(argc, argv);

    ls(files, config);
}
