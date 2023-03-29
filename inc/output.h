#include "config.h"
#include "fileinfo.h"
#include "libmx.h"

#include <sys/ioctl.h>
#include <time.h>

typedef struct {
    int links;
    int user;
    int group;
    int size;
} t_width;

int print_fileinfo(t_fileinfo *fileinfo, t_config *config);
void print_multicolumn(t_list *fileinfos, t_config *config);
void print_long(t_list *fileinfos, t_config *config);
void print_fileinfos(t_list *fileinfos, t_config *config);
