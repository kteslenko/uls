#pragma once

#include <stdbool.h>

typedef enum {
    FORMAT_SINGLECOLUMN, // -1
    FORMAT_MULTICOLUMN,  // -C
    FORMAT_STREAM,       // -m
    FORMAT_LONG,         // -l
} t_format;

typedef enum {
    TIME_MODIFICATION,  // default
    TIME_STATUS_CHANGE, // -c
    TIME_ACCESS,        // -u
} t_time_type;

typedef enum {
    SORT_NAME, // default
    SORT_TIME, // -t
    SORT_SIZE, // -S
    SORT_NONE, // -f
} t_sort_type;

typedef enum {
    IGNORE_HIDDEN, // default
    IGNORE_DOTS,   // -A
    IGNORE_NONE,   // -a
} t_ignore_type;

typedef struct {
    t_format format;
    t_time_type time_type;
    t_sort_type sort_type;
    t_ignore_type ignore_type;
    bool recursive;           // -R
    bool colorize;            // -G
    bool classify;            // -F
    bool slash;               // -p
    bool sort_reverse;        // -r
    bool human_readable;      // -h
    bool access_control_list; // -e
    bool complete_time_info;  // -T
    bool extended_attributes; // -@
} t_config;

t_config *parse_args(int argc, char *argv[]);
