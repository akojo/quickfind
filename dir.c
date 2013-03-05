#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dir.h"

int skip(char *dirname)
{
    return (dirname[0] == '.') &&
        ((dirname[1] == '\0') || (dirname[1] == '.' && dirname[2] == '\0'));
}

void dirwalk(char *dirname, int opts, void (*func)(char *, void *), void *ctx)
{
    DIR *dir;
    char *name;
    struct dirent *d;

    dir = opendir(dirname);
    if (!dir) return;

    name = malloc(FILENAME_MAX);

    while ((d = readdir(dir)) != NULL) {
        if (skip(d->d_name)) continue;
        if (d->d_name[0] == '.' && !(opts & DW_HIDDEN)) continue;

        sprintf(name, "%s/%s", dirname, d->d_name);

        switch (d->d_type) {
        case DT_DIR:
           if (opts & DW_DIRECTORIES) {
               func(name, ctx);
           }
           dirwalk(name, opts, func, ctx);
           break;
        case DT_REG:
        case DT_LNK:
           if (opts & DW_FILES) {
               func(name, ctx);
           }
           break;
        default:
           break;
        }
    }

    free(name);
    closedir(dir);
}
