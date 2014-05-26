#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "dir.h"

static int skip(char *dirname)
{
    return (dirname[0] == '.') &&
        ((dirname[1] == '\0') || (dirname[1] == '.' && dirname[2] == '\0'));
}

static int is_dir(struct dirent *d, char *name)
{
    struct stat s;
#if defined(DT_DIR) && defined(DT_UNKNOWN)
    return d->d_type == DT_DIR ||
        (d->d_type == DT_UNKNOWN && stat(name, &s) == 0 && S_ISDIR(s.st_mode));
#else
    return stat(name, &s) == 0 && S_ISDIR(s.st_mode);
#endif
}

static void walk(char *name, int opts, void (*func)(char *, void *), void *ctx)
{
    DIR *dir = opendir(name);
    int namelen = strlen(name);
    char* endp = name + namelen;
    struct dirent *d;

    if (!dir) return;

    *endp++ = '/';

    while ((d = readdir(dir)) != NULL) {
        if (skip(d->d_name)) continue;
        if (d->d_name[0] == '.' && !(opts & DW_HIDDEN)) continue;
        if (strlen(d->d_name) + namelen >= FILENAME_MAX) continue;

        strcpy(endp, d->d_name);

        if (is_dir(d, name)) {
           if (opts & DW_DIRECTORIES)
               func(name, ctx);
           walk(name, opts, func, ctx);
        }
        else if (opts & DW_FILES) {
            func(name, ctx);
        }
    }
    closedir(dir);
}

void dirwalk(char *dirname, int opts, void (*func)(char *, void *), void *ctx)
{
    char* buf = malloc(FILENAME_MAX);
    strcpy(buf, dirname);
    walk(buf, opts, func, ctx);
    free(buf);
}
