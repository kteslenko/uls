#include "libmx.h"

char *mx_file_to_str(const char *filename) {
    char *str = NULL;
    char buf[129];
    int file = open(filename, O_RDONLY);
    ssize_t result = -1;

    buf[128] = '\0';
    if (file != -1) {
        while ((result = read(file, buf, 128)) > 0) {
            char *temp = str;
            
            buf[result] = '\0';
            str = mx_strjoin(str, buf);
            mx_strdel(&temp);
        }
        close(file);
    }
    
    if (result == -1) {
        mx_strdel(&str);
    }
    return str;
}
