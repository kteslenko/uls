#include "config.h"

#include "libmx.h"

t_config *new_config() {
    t_config *config = malloc(sizeof(t_config));

    mx_memset(config, 0, sizeof(t_config));
    if (isatty(1)) {
        config->format = FORMAT_MULTICOLUMN;
    }
    return config;
}

t_config *parse_args(int argc, char *argv[]) {
    t_config *config = new_config();

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            break;
        }
        for (char *str = argv[i] + 1; *str != '\0'; str++) {
            switch (*str) {
            case 'R':
                config->recursive = true;
                break;
            case 'G':
                config->colorize = true;
                break;
            case 'F':
                config->classify = true;
                config->slash = false;
                break;
            case 'p':
                config->slash = true;
                config->classify = false;
                break;
            case 'r':
                config->sort_reverse = true;
                break;
            case 'h':
                config->human_readable = true;
                break;
            case 'e':
                config->access_control_list = true;
                break;
            case 'T':
                config->complete_time_info = true;
                break;
            case '@':
                config->extended_attributes = true;
                break;
            case 'g':
                config->omit_owner = true;
                break;
            case 'o':
                config->omit_group = true;
                break;
            case 'x':
                config->sort_across = true;
                break;
            case 'n':
                config->display_numeric = true;
                break;
            case 'q':
                config->non_printable = true;
                break;
            case 'H':
                config->follow_links = true;
                break;
            case '1':
                config->format = FORMAT_SINGLECOLUMN;
                break;
            case 'C':
                config->format = FORMAT_MULTICOLUMN;
                break;
            case 'm':
                config->format = FORMAT_STREAM;
                break;
            case 'l':
                config->format = FORMAT_LONG;
                break;
            case 'c':
                config->time_type = TIME_STATUS_CHANGE;
                break;
            case 'u':
                config->time_type = TIME_ACCESS;
                break;
            case 'U':
                config->time_type = TIME_CREATION;
                break;
            case 't':
                config->sort_type = SORT_TIME;
                break;
            case 'S':
                config->sort_type = SORT_SIZE;
                break;
            case 'f':
                config->sort_type = SORT_NONE;
                config->ignore_type = IGNORE_NONE;
                break;
            case 'A':
                config->ignore_type = IGNORE_DOTS;
                break;
            case 'a':
                config->ignore_type = IGNORE_NONE;
                break;
            default:
                mx_printerr("uls: illegal option -- ");
                write(2, str, 1);
                write(2, "\n", 1);
                mx_printerr("usage: uls [-@ACFGHRSTUacefghlmnopqrtux1] [file ...]\n");
                exit(1);
            }
        }
    }

    if (!isatty(1)) {
        config->colorize = false;
    } else {
        config->non_printable = true;
    }
    if (!config->follow_links) {
        config->follow_links =
            config->format != FORMAT_LONG && !config->colorize && !config->classify;
    }
    if (config->sort_type == SORT_NONE) {
        config->ignore_type = IGNORE_NONE;
    }

    return config;
}
