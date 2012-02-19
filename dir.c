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

    name = malloc(BUFSIZ); /* Should be enough for everybody */

    while ((d = readdir(dir)) != NULL) {
        if (d->d_name[0] == '.' && !(opts & DW_HIDDEN)) continue;
        if (skip(d->d_name)) continue;

        sprintf(name, "%s/%s", dirname, d->d_name);

        if (d->d_type == DT_DIR || (opts & DW_ALL))
            func(name, ctx);
        if (d->d_type == DT_DIR)
            dirwalk(name, opts, func, ctx);
    }

    free(name);
    closedir(dir);
}
