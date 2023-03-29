#pragma once

#include "fileinfo.h"
#include "libmx.h"

void sort_filenames(t_list *filenames, t_sort_type sort_type);
void sort_fileinfos(t_list *fileinfos, t_sort_type sort_type, bool reverse);
