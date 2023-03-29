#include "output.h"

static int max_width(t_list *fileinfos) {
    int width = 0;

    while (fileinfos != NULL) {
        t_fileinfo *fileinfo = fileinfos->data;
        int len = mx_strlen(fileinfo->name);

        if (width < len) {
            width = len;
        }
        fileinfos = fileinfos->next;
    }

    return width;
}

static int get_termwidth() {
    if (isatty(1)) {
        struct winsize w;
        ioctl(0, TIOCGWINSZ, &w);
        return w.ws_col;
    }
    return 80;
}

static void print_tabs(int width, int tabwidth) {
    int tabs = (width + tabwidth - 1) / tabwidth;

    for (int i = 0; i < tabs; i++) {
        if (tabwidth == 1) {
            mx_printchar(' ');
        } else {
            mx_printchar('\t');
        }
    }
}

static t_fileinfo **list_to_array(t_list *fileinfos, int *count) {
    *count = mx_list_size(fileinfos);

    if (count == 0) {
        return NULL;
    }

    t_fileinfo **array = malloc(sizeof(t_fileinfo *) * (*count));

    for (int i = 0; i < *count; i++) {
        array[i] = fileinfos->data;
        fileinfos = fileinfos->next;
    }

    return array;
}

void print_multicolumn(t_list *fileinfos, t_config *config) {
    int tabwidth = 8;
    if (config->colorize) {
        tabwidth = 1;
    }

    int width = max_width(fileinfos);
    if (config->classify || config->slash) {
        width++;
    }
    width = (width + tabwidth) & ~(tabwidth - 1);

    int count;
    t_fileinfo **array = list_to_array(fileinfos, &count);

    int num_cols = get_termwidth() / width;
    int num_rows = (count + num_cols - 1) / num_cols;

    int idx = 0;
    for (int i = 0; i < num_rows; i++) {
        if (!config->sort_across) {
            idx = i;
        }
        for (int j = 0; j < num_cols; j++) {
            int printed = print_fileinfo(array[idx], config);
            if (config->sort_across) {
                idx++;
            } else {
                idx += num_rows;
            }
            if (idx >= count) {
                break;
            }
            print_tabs(width - printed, tabwidth);
        }
        mx_printchar('\n');
    }

    if (array != NULL) {
        free(array);
    }
}
